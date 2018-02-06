#include "LevelLoader.h"


void operator >> (const YAML::Node& obstacleNode, WorldData& obstacle)
{
	obstacle.m_type = obstacleNode["type"].as<std::string>();
	obstacle.m_position.x = obstacleNode["position"]["x"].as<float>();
	obstacle.m_position.y = obstacleNode["position"]["y"].as<float>();
	obstacle.m_size.x = obstacleNode["size"]["width"].as<float>();
	obstacle.m_size.y = obstacleNode["size"]["height"].as<float>();
}

void operator >> (const YAML::Node& enemyNode, EnemyData& enemy)
{
	enemy.m_position.x = enemyNode["position"]["x"].as<float>();
	enemy.m_position.y = enemyNode["position"]["y"].as<float>();
}

/// <summary>
/// @brief Top level function that extracts various game data from the YAML data stucture.
/// 
/// Invokes other functions to extract the background, tank and obstacle data.
//   Because there are multiple obstacles, obstacle data are stored in a vector.
/// </summary>
/// <param name="tankNode">A YAML node</param>
/// <param name="tank">A simple struct to store data related to the player tank</param>
////////////////////////////////////////////////////////////
void operator >> (const YAML::Node& levelNode, LevelData& level)
{

	const YAML::Node& obstaclesNode = levelNode["world"].as<YAML::Node>();
	for (unsigned i = 0; i < obstaclesNode.size(); ++i)
	{
		WorldData obstacle;
		obstaclesNode[i] >> obstacle;
		level.m_worldPieces.push_back(obstacle);
	}

	const YAML::Node& enemyNode = levelNode["enemies"].as<YAML::Node>();
	for (unsigned i = 0; i < enemyNode.size(); ++i)
	{
		EnemyData enemy;
		enemyNode[i] >> enemy;
		level.m_enemies.push_back(enemy);
	}
}

////////////////////////////////////////////////////////////
bool LevelLoader::load(int nr, LevelData& level)
{
	std::stringstream ss;
	ss << "levels/level";
	ss << nr;
	ss << ".yaml";

	try
	{
		YAML::Node baseNode = YAML::LoadFile(ss.str());
		if (baseNode.IsNull())
		{
			std::string message("file: " + ss.str() + " not found");
			throw std::exception(message.c_str());
		}
		baseNode >> level;
	}
	catch (YAML::ParserException& e)
	{
		std::cout << e.what() << "\n";
		return false;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << "\n";
		return false;
	}

	return true;
}

