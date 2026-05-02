#include <print>
#include <stdexcept>
#include <string>
#include <thread>

std::exception_ptr g_exception = nullptr;

void thread_function()
{
  try
  {
    throw std::runtime_error("exception from std::thread");
  }
  catch (...)
  {
    g_exception = std::current_exception();
  }
}

int main()
{
  std::thread t(thread_function);

  t.join();

  if (g_exception)
  {
    try
    {
      std::rethrow_exception(g_exception);
    }
    catch (std::exception const & e)
    {
      std::print("main : caught exception : {}\n", e.what());
    }
  }
}