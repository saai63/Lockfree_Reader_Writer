#include <thread>
#include <atomic>
#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <mutex>

/* struct to model a tuple returned by read()
 *  Provides an utility to check the validity*/
typedef struct read_result_def
{
   std::string _str;
   uint32_t _u32;

   /* The object is deemed corrupted if the number and
     * its string representation are not in sync */
   bool bValid()
   {
      return (std::to_string(_u32) == _str);
   }
} read_result;

/* This models the nlohmann_json object.
  * Doesn't have any kind of locks internally
  * Simulates lookup times via sleep  */
class Config
{
private:
   std::string m_str;
   uint32_t m_u32;

public:
   Config(const std::string &str, uint32_t num) : m_str(str), m_u32(num)
   {
   }

   Config(const Config &other) : m_str(other.m_str), m_u32(other.m_u32)
   {
   }

   Config &operator=(Config &src)
   {
      m_str = src.m_str;
      m_u32 = src.m_u32;
      return *this;
   }

   void update(const std::string &str, uint32_t &num)
   {
      m_str = str;
      std::this_thread::sleep_for(std::chrono::milliseconds(3)); // This sleep simulates long search times if any
      m_u32 = num;
   }

   read_result read()
   {
      read_result temp;
      temp._str = m_str;
      std::this_thread::sleep_for(std::chrono::milliseconds(9)); // This sleep simulates long search times if any
      temp._u32 = m_u32;
      return temp;
   }
};

class ConfigReader
{
private:
   /* Hide the C-tor */
   ConfigReader()
   {
      m_readPtr = std::make_shared<Config>("0", 0);
   }
   std::shared_ptr<Config> m_readPtr;
   std::mutex m_mutex;

public:
   /* Meyer's singleton */
   static ConfigReader &getInstance()
   {
      static ConfigReader obj;
      return obj;
   }

   void update(const std::string &str, uint32_t &num)
   {
      m_mutex.lock();
      auto writePtr = std::make_shared<Config>(*m_readPtr); // Create a temporary copy
      writePtr->update(str, num);
      std::atomic_exchange(&m_readPtr, writePtr); // Both readPtr and writePtr now point to the heap object of writePtr
      m_mutex.unlock();
   }

   read_result read()
   {
      auto tmp = std::atomic_load(&m_readPtr); // Create a copy of the pointer which internally increments the use count
      return tmp->read();                      // Read from this copy. The copy ensures that underlying raw pointer isn't freed
   }
};

// ---------------------------------
// Thread functions
// ---------------------------------
void reader()
{
   for (uint32_t i = 0; i < 100; i++)
   {
      auto result = ConfigReader::getInstance().read();
      if (!result.bValid())
      {
         std::cout << "Data corrupted ! : [" << result._u32 << "]  [" << result._str.c_str() << "]" << std::endl;
         exit(1);
      }
      else
      {
         std::cout << "[" << result._u32 << "]  [" << result._str.c_str() << "]" << std::endl;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
   }
}

void writer()
{
   for (uint32_t i = 0; i < 100; i++)
   {
      ConfigReader::getInstance().update(std::to_string(i), i);
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
   }
   std::cout << "Writer done" << std::endl;
}

int main()
{
   std::vector<std::thread> readers;
   for (int n = 0; n < 20; n++)
   {
      readers.emplace_back(reader);
   }
   std::vector<std::thread> writers;
   for (int n = 0; n < 5; n++)
   {
      writers.emplace_back(writer);
   }

   for (auto &idx : readers)
   {
      idx.join();
   }
   for (auto &idx : writers)
   {
      idx.join();
   }

   return 0;
}
