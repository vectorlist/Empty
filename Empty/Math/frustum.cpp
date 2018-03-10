#include <PCH.h>
#include "frustum.h"

void Frustum::ExtractPlanes(const Matrix4x4& proj, const Matrix4x4& view)
{
	//mult proj view
	Matrix4x4 mvp = proj * view; //view * proj if u using comlum major


	//TODO must be smae opengl and direct ndc z value


	//test Z near (GL : 0.1999999, DX 0.1) now 0.1 for GL
	//NDC GL(-1 to 1) DX(0 to 1)

	float znear = -mvp[2][3] / mvp[2][2];

	

}
