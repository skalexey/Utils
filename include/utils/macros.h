#pragma once

#define WRAP_EXCEP_RETURN_FALSE(expr) { \
  try \
  { \
    expr; \
  } \
  catch (const std::exception& e) \
  { \
    return false; \
  } \
  catch (...) \
  { \
    return false; \
  } \
}