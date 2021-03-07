#include "CollisionManager.h"
#include "BasicMeshShape.h"
#include "Grid.h"
#include <cmath>
#include <iostream>
using namespace StarBangBang;

const unsigned int sides = 30;



CollisionData::CollisionData()
	:pen_depth{ 0.0f }, col_normal{ 0.0f, 0.0f } {}

CollisionPair::CollisionPair(BoxCollider& A, BoxCollider& B, CollisionData data) : A{ A }, B{ B }, data{ data }
{
}
namespace
{
	
	PartitionGrid p_grid ;
	std::queue<CollisionPair> resolveQueue;
	//all colliders created
	std::vector<BoxCollider> collider_list;
	
}


void CalculateCollisionData(const BoxCollider& b1, const BoxCollider& b2, CollisionData& col)
{
	AEVec2 dist = AEVec2{ b2.GetCenter().x - b1.GetCenter().x , b2.GetCenter().y - b1.GetCenter().y };
	float x_intersect = b1.GetExtend().x + b2.GetExtend().x - fabs(dist.x);
	float y_intersect = b1.GetExtend().y + b2.GetExtend().y - fabs(dist.y);

	//Find the min intersect distance 
	if (x_intersect > y_intersect)
	{
		col.pen_depth = x_intersect;
		// means that b2 center on the left of b1 center 
		if (dist.x < 0)
			col.col_normal = AEVec2{ 1.0f,0.0f };
		else
			col.col_normal = AEVec2{ -1.0f,0.0f };
	}
	else
	{
		col.pen_depth = y_intersect;
		//means b2 center is below b1 center
		if (dist.y < 0)
			col.col_normal = AEVec2{ 0.0f, 1.0f };
		else
			col.col_normal = AEVec2{ 0.0f,-1.0f };

	}


}

bool CollisionManager::ContainsPoint(const BoxCollider& box, AEVec2 pt)
{
	if (pt.x < box.Min().x || pt.y < box.Min().x)
		return false;
	if (pt.x > box.Max().x || pt.y > box.Max().y)
		return false;
	return true;

}

void FetchAllColliderCells(const BoxCollider& c, std::vector<Cell*>& list)
{

	/*Cell& maxCell = p_grid.grid[p_grid.GetHashCellIndex(c.Max())];
	Cell& minCell = p_grid.grid[p_grid.GetHashCellIndex(c.Min())];
	AEVec2 dimension{ c.GetWidth(),c.GetHeight() };
	int x_limit = (int)ceil(dimension.x / p_grid.GetCellSize()) ;
	int y_limit = (int)ceil(dimension.y / p_grid.GetCellSize()) ;

	for (int y = -1; y < y_limit; ++y)
	{
		for (int x = -1; x < x_limit; ++x)
		{
			AEVec2 v{ c.Min().x + x * p_grid.GetCellSize() , c.Min().y + y * p_grid.GetCellSize() };

			
			int cellIndex = p_grid.GetHashCellIndex(v);
			Cell& cell = p_grid.grid[cellIndex];
			cell.cellIndex = cellIndex;
			list.push_back(&cell);
			

			

		}
	}*/
	
	std::vector<AEVec2> points;
	points.reserve(8);
	AEVec2 max = c.Max();
	AEVec2 min = c.Min();

	AEVec2 topR{ min.x, max.y };
	AEVec2 btmL{ max.x,min.y };
	AEVec2 v = min;
	//calcuate all in between points 
	for (int y = 0; ; ++y)
	{
		if (v.x > max.x || v.y > max.y)
			break;
		for (int x = 0; ; ++x)
		{
			v = AEVec2{ c.Min().x + x * p_grid.GetCellSize() , c.Min().y + y * p_grid.GetCellSize() };

			if (v.x > max.x || v.y > max.y)
				break;

			points.push_back(v);

		}
	}
	points.push_back(max);
	points.push_back(topR);
	points.push_back(btmL);

	for (size_t i = 0; i < points.size(); ++i)
	{
		int cellIndex = p_grid.GetHashCellIndex(points[i]);
		Cell& cell = p_grid.grid[cellIndex];
		cell.cellIndex = cellIndex;
		list.push_back(&cell);
	}

}
void CollisionManager::RecalculateColliderCells(BoxCollider& col)
{
	//clear the cells collider occupies
	if (col.GetCellIndexes().size() > 0)
	{
		for (const int index : col.GetCellIndexes())
		{
			Cell& c = p_grid.grid[index];
			c.cell_colliders.erase(&col);
		}
		//clear all cell data from collider
		col.ClearCellList();
	}

	//update the new cells collider occupies
	std::vector<Cell*> cell_list;
	FetchAllColliderCells(col, cell_list);
	for (Cell* cell_ref : cell_list)
	{
		if (!cell_ref)
			continue;

		cell_ref->cell_colliders.insert(&col);
		col.AddToCellList(cell_ref->cellIndex);

	}


}
void CollisionManager::AddToColliders(BoxCollider c)
{
	collider_list.push_back(c);
	unsigned int index = (unsigned int)collider_list.size() - 1;

	std::vector<AEVec2> points;
	points.reserve(8);
	AEVec2 max = c.Max();
	AEVec2 min = c.Min();

	AEVec2 topR{min.x, max.y};
	AEVec2 btmL{max.x,min.y};
	AEVec2 v = min;
	//calcuate all the in between points 
	for (int y = 0; ; ++y)
	{
		if (v.x > max.x || v.y > max.y)
			break;
		for (int x = 0; ; ++x)
		{
			 v = AEVec2{ c.Min().x + x * p_grid.GetCellSize() , c.Min().y + y * p_grid.GetCellSize() };

			if (v.x > max.x || v.y > max.y)
				break;

			points.push_back(v);

		}
	}
	points.push_back(max);
	points.push_back(topR);
	points.push_back(btmL);

	for (size_t i = 0; i < points.size(); ++i)
	{
		int cellIndex = p_grid.GetHashCellIndex(points[i]);
		Cell& cell = p_grid.grid[cellIndex];
		cell.cellIndex = cellIndex;
		collider_list[index].AddToCellList(cellIndex);
		cell.cell_colliders.insert(&collider_list[index]);
	}


	/*
	Cell& maxCell = p_grid.grid[p_grid.GetHashCellIndex(c.Max())];
	Cell& minCell = p_grid.grid[p_grid.GetHashCellIndex(c.Min())];
	AEVec2 dimension{ c.GetWidth(),c.GetHeight() };
	
	int x_limit = (int)ceil(dimension.x / p_grid.GetCellSize());
	int y_limit = (int)ceil(dimension.y / p_grid.GetCellSize()) ;

	for (int y = 0; y < y_limit ; ++y)
	{
		for (int x = 0 ; x < x_limit ; ++x)
		{
			AEVec2 v{ c.Min().x + x * p_grid.GetCellSize() , c.Min().y + y * p_grid.GetCellSize() };

			
			int cellIndex = p_grid.GetHashCellIndex(v);
			Cell& cell = p_grid.grid[cellIndex];
			cell.cellIndex = cellIndex;
			collider_list[index].AddToCellList(cellIndex);
			cell.cell_colliders.insert(&collider_list[index]);
			
			
		}
	}*/



}
void DrawParition()
{
	for (int y = 0; y < 32; ++y)
	{
		for (int x = 0; x < 32; ++x)
		{
			AEVec2 pos{ x * p_grid.GetCellSize(),y * p_grid.GetCellSize() };
			DrawBoxWired(AEVec2{ p_grid.GetCellSize(),p_grid.GetCellSize() }, pos);
		}
	}
}
//wip
void CollisionManager::ResolverUpdate()
{

	if (!resolveQueue.empty())
	{
		for (size_t i = 0; i < resolveQueue.size(); ++i)
		{
			CollisionPair& pair = resolveQueue.front();
			CollisionManager::Resolve(pair.A, pair.B, pair.data);
			resolveQueue.pop();
		}
	}

	//non-partition 
	/*for (BoxCollider& col : collider_list)
	{
		for (BoxCollider& col2 : collider_list)
		{
			if (&col == &col2)
				continue;
			CollisionData data;
			if (Dynamic_AABB(col, AEVec2{ 0,0 }, col2, AEVec2{ 0,0 }, data))
			{
				CollisionPair p{ col,col2, data };
				PRINT("Added to resolve queue\n");
				AddToResolveQueue(p);
			}

		}
		DebugCollider(col, Black());
	}*/


	//paritition (still have some bugs)
	for (BoxCollider& col : collider_list)
	{
		
		if (col.GetCellListSize() > 0)
		{
			for (const int index : col.GetCellIndexes())
			{
				Cell& c = p_grid.grid[index];

				for (BoxCollider* box : c.cell_colliders)
				{
					if (box == &col)
						continue;
					CollisionData data;
					if (Dynamic_AABB(col, AEVec2{ 0,0 }, *box, AEVec2{ 0,0 }, data))
					{
						CollisionPair p{ col,*box, data };
						AddToResolveQueue(p);

					}
				}

			}
		}

		
		if(col.isStatic)
			DebugCollider(col, Black());
		else
			DebugCollider(col, Red());
	}

	//DrawParition();
}
//wip
void CollisionManager::AddToResolveQueue(CollisionPair pair)
{
	
	resolveQueue.push(pair);

}
//check for static aabb
bool CollisionManager::StaticAABB_Check(const BoxCollider& A, const BoxCollider& B, CollisionData& data)
{

	if (A.isTrigger || B.isTrigger)
		return false;

	//A is outside B bounding box
	if (A.Min().x > B.Max().x || A.Min().y > B.Max().y)
	{
		return false;
	}
	//B is outside A bounding box
	if (B.Min().x > A.Max().x || B.Min().y > A.Max().y)
	{
		return false;
	}
	CalculateCollisionData(A, B, data);
	return true;
}

//check for moving aabbs
bool CollisionManager::Dynamic_AABB(const BoxCollider& A, const AEVec2& vel1,
	const BoxCollider& B, const AEVec2& vel2, CollisionData& data)
{

	if (vel1.x == 0 && vel1.y == 0 && vel2.x == 0 && vel2.y == 0)
	{
		return StaticAABB_Check(A, B, data);
	}

	if (StaticAABB_Check(A, B, data))
	{
		return true;
	
	}

	//first collision time //collision exit time
	float t_first = 0, t_last = (float)AEFrameRateControllerGetFrameTime();
	//B relative velocity to A (where A = aabb1,vel1 , B = aabb2 , vel2)
	AEVec2 rVel_B{ vel2.x - vel1.x,vel2.y - vel1.y };

	//x-axis
	//B moving left
	if (rVel_B.x < 0)
	{
		//A is on the right
		if (A.Min().x > B.Max().x)
			return false;
		//A is on the left
		if (A.Max().x < B.Min().x)
		{
			//curr collision time
			float f_temp = (A.Max().x - B.Min().x) / rVel_B.x;
			//take the max time compared to last frame
			t_first = max(f_temp, t_first);

		}
		//the frame where B is exiting A
		if (A.Min().x < B.Max().x)
		{
			//last collision time
			float l_temp = (A.Min().x - B.Max().x) / rVel_B.x;
			//take the min time compared to last frame
			t_last = min(l_temp, t_last);
		}


	}
	//B moving right
	if (rVel_B.x > 0)
	{
		//A is on the left
		if (A.Max().x < B.Min().x)
			return false;
		//A is on the right
		if (A.Min().x > B.Max().x)
		{
			float f_temp = (A.Min().x - B.Max().x) / rVel_B.x;
			t_first = max(f_temp, t_first);

		}
		//the frame where B is exiting A
		if (A.Max().x > B.Min().x)
		{
			float l_temp = (A.Max().x - B.Min().x) / rVel_B.x;
			t_last = min(l_temp, t_last);
		}

	}
	//no collision 
	if (t_first > t_last)
		return false;
	//y-axis
	//B moving down
	if (rVel_B.y < 0)
	{
		//A is on top
		if (A.Min().y > B.Max().y)
			return false;
		//A is on the bottom
		if (A.Max().y < B.Min().y)
		{
			float f_temp = (A.Max().y - B.Min().y) / rVel_B.y;
			t_first = max(f_temp, t_first);

		}
		//the frame where B is exiting A
		if (A.Min().y < B.Max().y)
		{
			float l_temp = (A.Min().y - B.Max().y) / rVel_B.y;
			t_last = min(l_temp, t_last);
		}

	}
	//B moving up
	if (rVel_B.y > 0)
	{
		//A is on the bottom
		if (A.Max().y < B.Min().y)
			return false;
		//A is on the top
		if (A.Min().y > B.Max().y)
		{
			float f_temp = (A.Min().y - B.Max().y) / rVel_B.y;
			t_first = max(f_temp, t_first);

		}
		//the frame where B is exiting A
		if (A.Min().y < B.Max().y)
		{
			float l_temp = (A.Max().y - B.Min().y) / rVel_B.y;
			t_last = min(l_temp, t_last);
		}

	}
	//no collision 
	if (t_first > t_last)
		return false;

	return true;


}

bool CollisionManager::CircleVsCircle(CircleCollider c1, CircleCollider c2, CollisionData& col)
{
	float r = c1.GetRadius() + c2.GetRadius();
	AEVec2 c1_center = c1.GetCenter();
	AEVec2 c2_center = c2.GetCenter();
	float d = AEVec2SquareDistance(&c1_center, &c2_center);
	if (d > r * r)
		return false;
	//collided
	d = sqrtf(d);

	if (d != 0)
	{
		col.pen_depth = r - d;
		col.col_normal = AEVec2{ (c2_center.x - c1_center.x) / d , (c2_center.y - c1_center.y) / d };
		return true;
	}
	//same circle center	
	col.pen_depth = c1.GetRadius();
	col.col_normal = AEVec2{ 0, 1 };
	return true;

}

void CollisionManager::DebugCollider(BoxCollider b, Color c)
{

	AEVec2 size = AEVec2{ b.GetWidth(),b.GetHeight() };
	StarBangBang::DrawBoxWired(size, b.GetCenter(), c);

}

void CollisionManager::DebugCollider(CircleCollider c)
{
	float interval = roundf(2.0f * PI / sides / 10.0f) * 10.0f;
	for (unsigned int i = 0; i < sides; i++)
	{
		float radian = interval * i;
		if (i + 1 < sides)
		{
			float x = c.GetRadius() * asinf(radian);
			float y = c.GetRadius() * acosf(radian);
			float x1 = c.GetRadius() * asinf(radian + interval);
			float y1 = c.GetRadius() * acosf(radian + interval);

			AEGfxLine(c.GetCenter().x + x, c.GetCenter().y + y, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
				c.GetCenter().x + x1, c.GetCenter().y + y1, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f

			);
		}
	}

}



