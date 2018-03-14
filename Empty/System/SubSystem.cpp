#include <PCH.h>
#include <System/subsystem.h>

namespace ECS
{	
SubSystemContext& Context()
{
	static SubSystemContext sContext;
	return sContext;
}

} 



