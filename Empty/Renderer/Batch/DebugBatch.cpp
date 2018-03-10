#include <PCH.h>
#include <renderer/batch/debugbatch.h>

DebugBatch*  DebugBatch::globalDebugBatch = nullptr;

DebugBatch::DebugBatch()
{
	if (globalDebugBatch) {
		ASSERT_MSG(0, "invalid one more debug renderer");
	}
	globalDebugBatch = this;
}

