#include <PCH.h>
#include <renderer/batch/FontBatch.h>

FontBatch* FontBatch::globalFontBatch = nullptr;

FontBatch::FontBatch()
{
	if (globalFontBatch != nullptr) {
		ASSERT_MSG(0,"invalid access to one more font batch");
	}
	globalFontBatch = this;
}

FontBatch::~FontBatch()
{
}

FontBatch* FontBatch::GetInstance()
{
	return globalFontBatch;
}
