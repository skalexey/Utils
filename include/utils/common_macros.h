#pragma once

#define RETURN_IF_NE_0(ex) { \
	auto retcode = ex; \
	if (retcode != 0) \
		return retcode; \
	}