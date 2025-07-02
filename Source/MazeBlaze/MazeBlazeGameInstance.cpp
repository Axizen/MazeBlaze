#include "MazeBlazeGameInstance.h"

UMazeBlazeGameInstance::UMazeBlazeGameInstance()
{
	// Default to player mode
	bPlayAsAI = false;
	
	// Default to frontier-based exploration
	AIExplorationSystem = EAIExplorationSystem::Frontier;
}

void UMazeBlazeGameInstance::SetPlayAsAI(bool bNewValue)
{
	bPlayAsAI = bNewValue;
}

bool UMazeBlazeGameInstance::GetPlayAsAI() const
{
	return bPlayAsAI;
}

void UMazeBlazeGameInstance::SetAIExplorationSystem(EAIExplorationSystem NewSystem)
{
	AIExplorationSystem = NewSystem;
}

EAIExplorationSystem UMazeBlazeGameInstance::GetAIExplorationSystem() const
{
	return AIExplorationSystem;
}

FText UMazeBlazeGameInstance::GetExplorationSystemDescription() const
{
	switch (AIExplorationSystem)
	{
	case EAIExplorationSystem::Frontier:
		return FText::FromString("Frontier-Based Exploration: Identifies boundaries between explored and unexplored areas and prioritizes exploration targets based on various factors.");
	case EAIExplorationSystem::GraphBased:
		return FText::FromString("Graph-Based Navigation: Builds a topological graph of the maze where nodes represent junctions and decision points, enabling more efficient navigation.");
	case EAIExplorationSystem::HierarchicalAStar:
		return FText::FromString("Hierarchical A* with Memory: Divides the maze into sectors and uses multi-level pathfinding for more efficient navigation in complex mazes.");
	case EAIExplorationSystem::PotentialField:
		return FText::FromString("Potential Field Navigation: Represents the maze as a field of attractive and repulsive forces, creating smooth and natural navigation paths.");
	case EAIExplorationSystem::WallFollowing:
		return FText::FromString("Memory-Augmented Wall Following: Enhances the classic wall-following algorithm with spatial memory and decision-making capabilities.");
	case EAIExplorationSystem::VisibilityGraph:
		return FText::FromString("Visibility Graph Navigation: Creates a navigation system based on visibility between points in the maze, optimizing for the shortest paths.");
	default:
		return FText::FromString("Unknown exploration system.");
	}
}

FText UMazeBlazeGameInstance::GetExplorationSystemPros() const
{
	switch (AIExplorationSystem)
	{
	case EAIExplorationSystem::Frontier:
		return FText::FromString("• Efficient exploration of unknown areas\n• Adapts well to maze complexity\n• Good balance between exploration and goal-seeking\n• Works well with partial information");
	case EAIExplorationSystem::GraphBased:
		return FText::FromString("• More memory-efficient than grid-based approaches\n• Better represents the actual structure of the maze\n• Enables intelligent backtracking\n• Can be visualized as a mini-map");
	case EAIExplorationSystem::HierarchicalAStar:
		return FText::FromString("• More efficient pathfinding in large mazes\n• Better handling of maze complexity\n• Reduces computational cost for long-distance planning\n• Adapts to dynamic maze elements");
	case EAIExplorationSystem::PotentialField:
		return FText::FromString("• Smooth and natural-looking navigation\n• Handles dynamic obstacles well\n• Creates interesting emergent behaviors\n• Easily combined with other methods");
	case EAIExplorationSystem::WallFollowing:
		return FText::FromString("• Simple to implement but effective\n• Low computational overhead\n• Guaranteed to solve any simply connected maze\n• Can be combined with higher-level planning");
	case EAIExplorationSystem::VisibilityGraph:
		return FText::FromString("• Finds optimal paths through complex environments\n• Works well in mazes with open areas\n• Can handle non-grid-based maze layouts\n• Efficient for repeated navigation");
	default:
		return FText::FromString("No information available.");
	}
}

FText UMazeBlazeGameInstance::GetExplorationSystemCons() const
{
	switch (AIExplorationSystem)
	{
	case EAIExplorationSystem::Frontier:
		return FText::FromString("• Can be computationally expensive in large mazes\n• May revisit areas unnecessarily\n• Requires careful tuning of priority functions\n• Grid-based approach can use significant memory");
	case EAIExplorationSystem::GraphBased:
		return FText::FromString("• More complex to implement\n• Junction detection can be challenging\n• May miss optimal paths in certain maze layouts\n• Requires good heuristics for decision making");
	case EAIExplorationSystem::HierarchicalAStar:
		return FText::FromString("• Complex implementation\n• Requires preprocessing or dynamic sector creation\n• May find suboptimal paths in some cases\n• Higher memory requirements for sector data");
	case EAIExplorationSystem::PotentialField:
		return FText::FromString("• Can get trapped in local minima\n• Field calculation can be computationally expensive\n• Requires careful tuning of force parameters\n• May oscillate in narrow passages");
	case EAIExplorationSystem::WallFollowing:
		return FText::FromString("• Not optimal for path finding\n• Can be inefficient in open areas\n• May get confused in complex mazes\n• Limited strategic planning capability");
	case EAIExplorationSystem::VisibilityGraph:
		return FText::FromString("• Expensive visibility calculations\n• Struggles in maze-like environments with limited visibility\n• Graph construction can be complex\n• Not ideal for initial exploration");
	default:
		return FText::FromString("No information available.");
	}
}