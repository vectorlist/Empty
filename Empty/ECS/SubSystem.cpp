#include <PCH.h>
#include <ECS/subsystem.h>

namespace ECS
{	
SubSystemContext& Context()
{
	static SubSystemContext sContext;
	return sContext;
}

} 



