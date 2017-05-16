#include "FrameWork.h"
#include <fstream>
#include "AgentConfig.h"
#include "SceneConfig.h"
#include "ColorConfig.h"
#include "CrowdSimulationConfig.h"
#include "SimulationCommon.h"
#include "SceneGrid.h"
#include "UniformGridScene.h"
#include "QuadTreeScene.h"
#include "Group.h"
#include "DerivedAgent.h"
#include "VisualizationByOpenCV.h"
#include "AStarPathFinding.h"

using namespace hiveCrowdSimulation;

CFrameWork::CFrameWork() : m_pScene(NULL), m_pCrowdSimulation(NULL), m_pVisualization(NULL), m_IsInit(false), m_IsColorConfigParsed(false)
{
}

CFrameWork::~CFrameWork()
{
	delete m_pCrowdSimulation;
	delete m_pVisualization;
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::initFramework(const std::string& vAgentInfo, const std::string& vSceneInfo, const std::string& vSimulationInfo)
{
	CSceneConfig::getInstance()->parseV(vSceneInfo);
	CAgentConfig::getInstance()->parseV(vAgentInfo);
	CCrowdSimulationConfig::getInstance()->parseV(vSimulationInfo);

	__initSceneInfo();
	__initSimulator();
    __initAgentInfo();
	//__initNoGroupAgentInfoByFile("crowdinfo.txt");
	//__initGroupInfoByFile("GroupInfo.txt");
	__initGraphInfo();
	__initPathFindingInfo();
	__initVisualizationInfo();

	m_IsInit = true;
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::initColor(const std::string& vDisplayColorInfo)
{
	CColorConfig::getInstance()->parseV(vDisplayColorInfo);
	m_IsColorConfigParsed = true;
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::doSimulation()
{
	if (!m_IsInit)
	{
		return;
	}

	int i = 0;
	while (!m_pCrowdSimulation->isFinished())
	{
		if (CCrowdSimulationConfig::getInstance()->getAttribute<bool>(SIMULATION_CONFIG_KEYWORD::IS_SAVE_SIMULATION_INFO))
		{
			if (i == 0)
			{
				__recordSimulationResult();
			}
		}
		__updateVisualization();
		m_pCrowdSimulation->advance();
		i++;
	}
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__initSimulator()
{
	m_pCrowdSimulation = new CCrowdSimulation();
	m_pCrowdSimulation->setScene(m_pScene);	
	m_pCrowdSimulation->initSimulator();
	m_pCrowdSimulation->addObstacleToRVO();
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__initNoGroupAgentInfoByFile(const std::string& vAgentFile)
{
	std::ifstream Fin(vAgentFile);
	unsigned int NumTotalAgent = 0;
	Fin >> NumTotalAgent;

	RVO::RVOSimulator* pRVOSimulator = m_pCrowdSimulation->getRVOSimulator();
	float MaxSpeed = CAgentConfig::getInstance()->getAttribute<float>(AGENT_CONFIG_KEYWORD::MAX_SPEED);
	for (unsigned int i=0; i<NumTotalAgent; ++i)
	{
		glm::vec2 Pos, Vel;
		bool IsAware;
		Fin >> Pos.x >>  Pos.y >>  Vel.x >>  Vel.y >> IsAware;
		CDerivedAgent* pOneAgent = new CDerivedAgent(Pos, Vel);
		m_pCrowdSimulation->addNoGroupAgent(pOneAgent);
		int RVOIndex = m_pCrowdSimulation->addAgent2RVOSimulatorAndReturnRVOIndex(pOneAgent);
		float MoveSpeed = MaxSpeed;
		pOneAgent->initAgentExtraInfo(RVOIndex, pRVOSimulator->getAgent(RVOIndex), MoveSpeed, Pos, m_pScene);
		if (!IsAware) 
		{
			pOneAgent->setIgnorance();
			MoveSpeed = hiveCommon::hiveGenRandomFloat(2.0, MoveSpeed);
		}
	}
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__initGroupInfoByFile(const std::string& vGroupFile)
{
	std::ifstream Fin(vGroupFile);
	unsigned int TotalNumGroups = 0;
	Fin >> TotalNumGroups;

	float MaxSpeed = CAgentConfig::getInstance()->getAttribute<float>(AGENT_CONFIG_KEYWORD::MAX_SPEED);
	RVO::RVOSimulator* pSimulator = m_pCrowdSimulation->getRVOSimulator();
	for (unsigned int i=0; i<TotalNumGroups; ++i)
	{
		CGroup* pOneGroup = new CGroup();
		unsigned int NumGroupAgents = 0;
		Fin >> NumGroupAgents;
		for (unsigned int k=0; k<NumGroupAgents; ++k)
		{
			glm::vec2 GroupMemberPos, GroupMemberVelocity;
			bool IsAware;
			Fin >> GroupMemberPos.x >> GroupMemberPos.y >> GroupMemberVelocity.x >> GroupMemberVelocity.y >> IsAware;
			CDerivedAgent* pGroupMember = new CDerivedAgent(GroupMemberPos, GroupMemberVelocity);
			int RVOIndex = m_pCrowdSimulation->addAgent2RVOSimulatorAndReturnRVOIndex(pGroupMember);
			pGroupMember->initAgentExtraInfo(RVOIndex, pSimulator->getAgent(RVOIndex), MaxSpeed, GroupMemberPos, m_pScene);
			if (!IsAware) 
			{
				pGroupMember->setIgnorance();
			}
			pGroupMember->setGroupID(i);
			pOneGroup->addGroupMember(pGroupMember);
		}
		
		m_pCrowdSimulation->addGroup(pOneGroup);
	}
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__initAgentInfo()
{
	unsigned int NumTotalAgent     = CCrowdSimulationConfig::getInstance()->getAttribute<int>(SIMULATION_CONFIG_KEYWORD::NUM_TOTAL_AGENT);
	unsigned int NumAwarenessAgent = NumTotalAgent * CCrowdSimulationConfig::getInstance()->getAttribute<float>(SIMULATION_CONFIG_KEYWORD::AWARENESS_PERCENTAGE);
	unsigned int NumIgnoranceAgent = NumTotalAgent * CCrowdSimulationConfig::getInstance()->getAttribute<float>(SIMULATION_CONFIG_KEYWORD::IGNORANCE_PERCENTAGE);
	unsigned int NumNoGroupMembers = CCrowdSimulationConfig::getInstance()->getAttribute<int>(SIMULATION_CONFIG_KEYWORD::NUM_NO_GROUP_MEMBERS);
	unsigned int NumGroups         = CCrowdSimulationConfig::getInstance()->getAttribute<int>(SIMULATION_CONFIG_KEYWORD::NUM_GROUPS);
	unsigned int MinNumGroupMembers = 0, MaxNumGroupMembers = 0;
	if (NumGroups > 0)
	{
		MinNumGroupMembers = CCrowdSimulationConfig::getInstance()->getAttribute<int>(SIMULATION_CONFIG_KEYWORD::FLOOR_GROUPMEMBERS);
		MaxNumGroupMembers = CCrowdSimulationConfig::getInstance()->getAttribute<int>(SIMULATION_CONFIG_KEYWORD::UPPER_GROUPMEMEBRS);
	}

	__initCrowdDistribution(NumTotalAgent, NumIgnoranceAgent, NumNoGroupMembers, NumGroups, MinNumGroupMembers, MaxNumGroupMembers);
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__initCrowdDistribution(unsigned int vNumTotalAgent, unsigned int vNumIgnorantAgent, unsigned int vNumNotGroupMembers, unsigned int vNumGroups, unsigned int vMinNumGroupMembers, unsigned int vMaxNumGroupMembers)
{
	__initNotGroupMemberDistribution(vNumNotGroupMembers);
	if (vNumGroups > 0)
	{
		__initGroupDistribution(vNumTotalAgent - vNumNotGroupMembers, vNumGroups, vMinNumGroupMembers, vMaxNumGroupMembers);
	}
	__initIgnorantAgent(vNumIgnorantAgent);
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__initNotGroupMemberDistribution(unsigned int vNumAgent)
{
	_ASSERT(m_pScene && m_pCrowdSimulation);

	for (unsigned int i=0; i<vNumAgent; ++i)
	{
		CDerivedAgent*     pOneAgent     = __genOneAgent();
		RVO::RVOSimulator* pRVOSimulator = m_pCrowdSimulation->getRVOSimulator();
		_ASSERT(pRVOSimulator);
		m_pCrowdSimulation->addNoGroupAgent(pOneAgent);
		int RVOIndex = m_pCrowdSimulation->addAgent2RVOSimulatorAndReturnRVOIndex(pOneAgent);
		pOneAgent->initAgentExtraInfo(RVOIndex, pRVOSimulator->getAgent(RVOIndex), CAgentConfig::getInstance()->getAttribute<float>(AGENT_CONFIG_KEYWORD::MAX_SPEED), pOneAgent->getInitPos(), m_pScene);
	}
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__initGroupDistribution(unsigned int vNumAllGroupMembers, unsigned int vNumGroup, unsigned int vMinNumGroupMembers, unsigned int vMaxNumGroupMembers)
{
	__decideGroupSize(vNumAllGroupMembers, vNumGroup, vMinNumGroupMembers, vMaxNumGroupMembers);
	for (unsigned int i=0; i<m_pCrowdSimulation->getNumGroups(); ++i)
	{
		__initGroupMembers(m_pCrowdSimulation->getGroup(i), i);
		__initRelationship(m_pCrowdSimulation->getGroup(i));
	}
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__decideGroupSize(int vNumAllGroupMembers, int vNumGroup, int vMinNumGroupMembers, int vMaxNumGroupMembers)
{
	int CurMin      = vMinNumGroupMembers;
	int CurMax      = vMaxNumGroupMembers;
	int RestMembers = vNumAllGroupMembers;
	int TempNumGroupMembers = 0;
	int GeneratedGroupMembers = 0;

	for (int i=0; i<vNumGroup; ++i)
	{
		CurMin = (vMinNumGroupMembers > RestMembers-vMaxNumGroupMembers*(vNumGroup-i-1) ? vMinNumGroupMembers : RestMembers-vMaxNumGroupMembers*(vNumGroup-i-1));
		CurMax = (vMaxNumGroupMembers < RestMembers-vMinNumGroupMembers*(vNumGroup-i-1) ? vMaxNumGroupMembers : RestMembers-vMinNumGroupMembers*(vNumGroup-i-1));
		TempNumGroupMembers = hiveCommon::hiveGenRandomInt(CurMin, CurMax);
		RestMembers -= TempNumGroupMembers;

		CGroup* pGroup = new CGroup();
		pGroup->setGroupSize(TempNumGroupMembers);
		_ASSERT(m_pCrowdSimulation);
		m_pCrowdSimulation->addGroup(pGroup);
	}
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__initRelationship(CGroup* vGroup)
{
	_ASSERT(vGroup);

	for (unsigned int i=0; i<vGroup->getNumGroupMembers(); ++i)
	{
		for (unsigned int k=0; k<vGroup->getNumGroupMembers(); ++k)
		{
			SAgentRelationship* pRelationship = new SAgentRelationship();

			if (i != k)
			{
				pRelationship->Intimacy = hiveCommon::hiveGenRandomFloat(1.0, 2.0);//Todo:亲密度

				SConcernedAgentInfo* pInfo = new SConcernedAgentInfo();
				pInfo->pConcernedAgent     = vGroup->getGroupMember(k);
				pInfo->LastCheckedPosition = vGroup->getGroupMember(k)->getPosition();
				pInfo->LastCheckedTimeStep = 0;
				vGroup->getGroupMember(i)->addConcernedAgentInfo(pInfo);
			}
			vGroup->setRelationship(vGroup->getGroupMember(i), vGroup->getGroupMember(k), pRelationship);//Todo:此处传人还是index比较好？？？
		}
	}
}

//********************************************************************
//FUNCTION:
CDerivedAgent* hiveCrowdSimulation::CFrameWork::__genOneAgent()
{
	std::vector<const hiveSceneProcess::SRectShape*> PassableRectSet;
	m_pScene->dumpPassableSet(PassableRectSet);
	_ASSERT(PassableRectSet.size() >= 1);
	int RectIndex = hiveCommon::hiveGenRandomInt(0, PassableRectSet.size() - 1);

	const hiveSceneProcess::SRectShape* pRect = PassableRectSet[RectIndex];
	hiveSceneProcess::SRectShape* pGenAgentRect = new hiveSceneProcess::SRectShape(pRect->m_LeftUp, pRect->m_RightDown);
	__repairGenAgentRect(pGenAgentRect);

	glm::vec2 AgentPos;
	AgentPos.x = hiveCommon::hiveGenRandomFloat(pGenAgentRect->m_LeftUp.x, pGenAgentRect->m_RightDown.x);
	AgentPos.y = hiveCommon::hiveGenRandomFloat(pGenAgentRect->m_LeftUp.y, pGenAgentRect->m_RightDown.y);
	glm::vec2 AgentDir(hiveCommon::hiveGenRandomFloat(-1.0, 1.0), hiveCommon::hiveGenRandomFloat(-1.0, 1.0));

	int TestWidth = m_pScene->getSceneWidthInPixel();
	int TestHeight = m_pScene->getSceneHeightInPixel();
	_ASSERT(AgentPos.x >= 0 && AgentPos.x < TestWidth);
	_ASSERT(AgentPos.y >= 0 && AgentPos.y < TestHeight);
	hiveCrowdSimulation::CDerivedAgent* pTempAgent = new hiveCrowdSimulation::CDerivedAgent(AgentPos, AgentDir);
	hiveSceneProcess::CSceneGrid* pSpecifiedGrid = m_pScene->getSceneGridV(AgentPos);
	pSpecifiedGrid->addAgent(pTempAgent);

	delete pGenAgentRect;
	return pTempAgent;
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__initGroupMembers(CGroup* vGroup, unsigned int vGroupID)
{
	_ASSERT(vGroup && m_pScene);

	RVO::RVOSimulator* pRVOSimulator = m_pCrowdSimulation->getRVOSimulator();

	for (unsigned int i=0; i<vGroup->getNumGroupMembers(); ++i)
	{
		CDerivedAgent* pOneGroupMember =  __genOneAgent();
		int RVOIndex = m_pCrowdSimulation->addAgent2RVOSimulatorAndReturnRVOIndex(pOneGroupMember);
		pOneGroupMember->initAgentExtraInfo(RVOIndex, pRVOSimulator->getAgent(RVOIndex), CAgentConfig::getInstance()->getAttribute<float>(AGENT_CONFIG_KEYWORD::MAX_SPEED), glm::vec2(-1, -1), m_pScene);
		pOneGroupMember->setGroupID(vGroupID);
		vGroup->setGroupMember(i, pOneGroupMember);
	}
	__repairGroupDistanceLimit(vGroup);
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__limitPosInScene(glm::vec2& voPos)
{
	float RepairSpace = CAgentConfig::getInstance()->getAttribute<float>(AGENT_CONFIG_KEYWORD::RADIUS);
	while (voPos.x < RepairSpace)
	{
		voPos.x++;
	}
	while (voPos.x > m_pScene->getSceneWidthInPixel())
	{
		voPos.x--;
	}
	while (voPos.y < RepairSpace)
	{
		voPos.y++;
	}
	while (voPos.y > m_pScene->getSceneHeightInPixel())
	{
		voPos.y--;
	}
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__repairGroupDistanceLimit(CGroup* vGroup)
{
	_ASSERT(vGroup->getNumGroupMembers() > 0);
	CDerivedAgent* pFirstGroupMember = vGroup->getGroupMember(0);

	float NeighborDist  = CAgentConfig::getInstance()->getAttribute<float>(AGENT_CONFIG_KEYWORD::NEIGHBOR_DIST);
	glm::vec2 LowRange  = pFirstGroupMember->getPosition() - NeighborDist;
	glm::vec2 HighRange = pFirstGroupMember->getPosition() + NeighborDist;

	__limitPosInScene(LowRange);
	__limitPosInScene(HighRange);

	for (unsigned int i=1; i<vGroup->getNumGroupMembers(); ++i)
	{
		bool ConditionA, ConditionB, ConditionC;
		glm::vec2 OtherGroupMemberPos;
		do 
		{
			OtherGroupMemberPos = glm::vec2(hiveCommon::hiveGenRandomFloat(LowRange.x, HighRange.x), hiveCommon::hiveGenRandomFloat(LowRange.y, HighRange.y));
			hiveSceneProcess::CSceneGrid* pSceneGrid = m_pScene->getSceneGridV(OtherGroupMemberPos);
			ConditionA = glm::distance(pFirstGroupMember->getPosition(), OtherGroupMemberPos) <= NeighborDist;
			ConditionB = pSceneGrid->isPassable();
			ConditionC = false;
			if (ConditionB)
			{
				ConditionC = m_pScene->isDirectVisibleV(pFirstGroupMember->getPosition(), OtherGroupMemberPos);
			}

		} while (!(ConditionA && ConditionB && ConditionC));

		vGroup->getGroupMember(i)->setPosition(OtherGroupMemberPos);
	}
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__repairGenAgentRect(hiveSceneProcess::SRectShape* voRect)
{
	//Todo: 方案1. 在所有格子的内部缩小R
	float Radius = CAgentConfig::getInstance()->getAttribute<float>(AGENT_CONFIG_KEYWORD::RADIUS);
	Radius += 4;
	voRect->m_LeftUp.x += Radius;
	voRect->m_LeftUp.y += Radius;

	voRect->m_RightDown.x -= Radius;
	voRect->m_RightDown.y -= Radius;

	//方案2.只在场景四条边的边界缩小范围
	//if (voRect->m_LeftUp.x == 0)
	//{
	//	voRect->m_LeftUp.x += 1.0;
	//}

	//if (voRect->m_RightDown.x == m_pScene->getSceneWidthInPixel())
	//{
	//	voRect->m_RightDown.x -= 1.0;
	//}

	//if (voRect->m_LeftUp.y == 0)
	//{
	//	voRect->m_LeftUp.y += 1.0;
	//}

	//if (voRect->m_RightDown.y == m_pScene->getSceneHeightInPixel())
	//{
	//	voRect->m_RightDown.y -= 1.0;
	//}
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__initIgnorantAgent(unsigned int vNumIgnorantAgent)
{
	std::vector<CDerivedAgent*> TotalAgentSet;
	m_pCrowdSimulation->dumpTotalAgent(TotalAgentSet);

	std::map<int, int> EffectiveCrowdSetIndexMap;
	for (unsigned int i=0; i<TotalAgentSet.size(); ++i)
	{
		EffectiveCrowdSetIndexMap.insert(std::pair<int, int>(i, i));
	}

	for (unsigned int i=0; i<vNumIgnorantAgent; ++i)
	{
		int MapIndex = hiveCommon::hiveGenRandomInt(0, EffectiveCrowdSetIndexMap.size() - 1);
		_ASSERT(EffectiveCrowdSetIndexMap.count(MapIndex) > 0);

		int AgentIndex = EffectiveCrowdSetIndexMap[MapIndex];
		EffectiveCrowdSetIndexMap.erase(MapIndex);

		std::map<int, int> NewEffectiveCrowdSetIndexMap;
		for (auto& Iter = EffectiveCrowdSetIndexMap.begin(); Iter != EffectiveCrowdSetIndexMap.end(); ++Iter)
		{
			NewEffectiveCrowdSetIndexMap.insert(std::pair<int, int>(NewEffectiveCrowdSetIndexMap.size(), (*Iter).second));
		}

		EffectiveCrowdSetIndexMap.clear();
		EffectiveCrowdSetIndexMap = NewEffectiveCrowdSetIndexMap;
		CDerivedAgent* pAgent = TotalAgentSet[AgentIndex];
		pAgent->setIgnorance();
		//_ASSERT(CAgentConfig::getInstance()->getAttribute<float>(AGENT_CONFIG_KEYWORD::MAX_SPEED) > 2.0);
		//pAgent->setMoveSpeed(hiveCommon::hiveGenRandomFloat(2.0, CAgentConfig::getInstance()->getAttribute<float>(AGENT_CONFIG_KEYWORD::MAX_SPEED)));
		//pAgent->setMaxSpeed(pAgent->getMoveSpeed());
	}
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__initGraphInfo()
{
	hiveSceneProcess::CGraphWithCrowd*    pGraphWithCrowd    = new hiveSceneProcess::CGraphWithCrowd();
	hiveSceneProcess::CGraphWithoutCrowd* pGraphWithoutCrowd = new hiveSceneProcess::CGraphWithoutCrowd;
	pGraphWithCrowd->genGraph(m_pScene);
	pGraphWithoutCrowd->genGraph(m_pScene);
	m_pCrowdSimulation->setGraph(pGraphWithCrowd, pGraphWithoutCrowd);
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__initPathFindingInfo()
{
	hivePathFinding::CBasePathFinding* pPathFindMethod = new hivePathFinding::CAStarPathFinding();
	m_pCrowdSimulation->setPathFindMethod(pPathFindMethod);
	m_pCrowdSimulation->initPathFindMethod();
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__initVisualizationInfo()
{
	_ASSERT(m_IsColorConfigParsed);
	glm::vec3 GLMBackColor = CColorConfig::getInstance()->getBackGroundColor();
	cv::Scalar BackColor   = cv::Scalar(GLMBackColor.x, GLMBackColor.y, GLMBackColor.z);
	m_pVisualization       = new hiveCrowdRendering::CVisualizationByOpenCV(m_pScene->getSceneWidthInPixel(), m_pScene->getSceneHeightInPixel(), BackColor);
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__recordSimulationResult()
{
	std::string SaveResultFile = CCrowdSimulationConfig::getInstance()->getAttribute<std::string>(SIMULATION_CONFIG_KEYWORD::SAVE_SIMULATION_FILE);
	SaveResultFile = strlwr(const_cast<char*>(SaveResultFile.c_str()));

	//std::vector<CDerivedAgent*> TotalAgent;
	//m_pCrowdSimulation->dumpTotalAgent(TotalAgent);

	//std::ofstream Fout(SaveResultFile);
	//Fout << TotalAgent.size() << std::endl;
	//for (auto& OneAgent : TotalAgent)
	//{
	//	Fout << OneAgent->getPosition().x << " " << OneAgent->getPosition().y << " " << OneAgent->getVelocity().x << " " << OneAgent->getVelocity().y << " " << OneAgent->isAware() << std::endl;
	//}
	//Fout.close();

	std::ofstream Fout(SaveResultFile);
	Fout << m_pCrowdSimulation->getNumGroups() << std::endl;
	for (unsigned int i=0; i<m_pCrowdSimulation->getNumGroups(); ++i)
	{
		CGroup* pOneGroup = m_pCrowdSimulation->getGroup(i);
		Fout << pOneGroup->getNumGroupMembers() << std::endl;
		for (unsigned int k=0; k<pOneGroup->getNumGroupMembers(); ++k)
		{
			CDerivedAgent* pOneGroupMember = pOneGroup->getGroupMember(k);
			Fout << pOneGroupMember->getPosition().x << " " << pOneGroupMember->getPosition().y << " " << pOneGroupMember->getVelocity().x << " " << pOneGroupMember->getVelocity().y << " " << pOneGroupMember->isAware() << std::endl;
		}
	}
	Fout.close();
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__drawGroupAgents(const std::vector<CGroup*>& vGroupAgentSet, const glm::vec3& vGroupColor)
{
	for (unsigned int i=0; i<vGroupAgentSet.size(); ++i)
	{
		glm::vec3 OneGroupColor = vGroupColor / (float)(i+1);
		for (unsigned int k=0; k<vGroupAgentSet[i]->getNumGroupMembers(); ++k)
		{
			hiveCrowdSimulation::CDerivedAgent* pOneGroupMember = vGroupAgentSet[i]->getGroupMember(k);
			bool IsFilled = pOneGroupMember->isAware() ? true : false;
			glm::vec2 Pos = pOneGroupMember->getPosition();
			m_pVisualization->drawRectangle(glm::vec2(Pos.x - 4, Pos.y - 4), glm::vec2(Pos.x + 4, Pos.y + 4), OneGroupColor, IsFilled);

			//if (!pOneGroupMember->isAware())
			//{
			//	std::vector<glm::vec2> RoadMapSet;
			//	pOneGroupMember->dumpRoadMap(RoadMapSet);
			//	__drawRoadMap(RoadMapSet, vGroupColor);
			//}

 			//if (Pos.x > 416 && Pos.x < 448 && Pos.y > 224 && Pos.y < 256)
 			//{
 			//	std::cout << pOneGroupMember->getRVOIndex() << std::endl;
 			//}

			if (pOneGroupMember->getRVOIndex() == 0)
			{
				m_pVisualization->drawRectangle(glm::vec2(Pos.x - 4, Pos.y - 4), glm::vec2(Pos.x + 4, Pos.y + 4), glm::vec3(0, 255, 0), IsFilled);

				std::vector<glm::vec2> RoadMapSet;
				pOneGroupMember->dumpRoadMap(RoadMapSet);
				__drawRoadMap(RoadMapSet, OneGroupColor);
			}
		}
	}
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__drawNoGroupAgents(const std::vector<CDerivedAgent*>& vNoGroupAgentSet, const glm::vec3& vAgentColor)
{
	float AgentRadius = CAgentConfig::getInstance()->getAttribute<float>(AGENT_CONFIG_KEYWORD::RADIUS);

	int i = 0;
	for (auto& Agent : vNoGroupAgentSet)
	{
		bool IsFilled = Agent->isAware();
		m_pVisualization->drawPoint(Agent->getPosition(), AgentRadius, vAgentColor, IsFilled);

		//glm::vec2 Pos = Agent->getPosition();
		//if (Pos.x > 192 && Pos.x < 230 && Pos.y > 380 && Pos.y < 416)
		//{
		//	std::cout << i << std::endl;
		//	m_pVisualization->drawPoint(Agent->getPosition(), AgentRadius, glm::vec3(0, 0, 255), IsFilled);
		//}
		//i++;
	}

// 	CDerivedAgent* Agent = vNoGroupAgentSet[364];
// 	m_pVisualization->drawPoint(Agent->getPosition(), AgentRadius, glm::vec3(0, 0, 255), Agent->isAware());
// 	glm::vec2 Pos = Agent->getPosition();
// 	std::cout << Pos.x << " " << Pos.y << std::endl;
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__updateVisualization()
{
	__clearScene();
	__drawScene();
	__drawExit();
	__drawCrowd();
	__display();
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__drawExit()
{
	_ASSERT(m_pVisualization && m_pScene);

	glm::vec3 ExitColor = CColorConfig::getInstance()->getExitColor();//Todo: magic number:出口半径
	for (unsigned int i=0; i<m_pScene->getNumExit(); ++i)
	{
		m_pVisualization->drawPoint(m_pScene->getExit(i), 5, ExitColor, true);
	}
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__drawScene()
{
	_ASSERT(m_pScene);

	std::vector<const hiveSceneProcess::SRectShape*> ObstacleRectSet, PassableRectSet;
	m_pScene->dumpObstacleSet(ObstacleRectSet);
	m_pScene->dumpPassableSet(PassableRectSet);

	glm::vec3 ObstacleColor = CColorConfig::getInstance()->getObstacleGridColor();
	glm::vec3 PassableColor = CColorConfig::getInstance()->getPassGridColor();

	for (auto& ObsRect : ObstacleRectSet)
	{
		m_pVisualization->drawRectangle(ObsRect->m_LeftUp, ObsRect->m_RightDown, ObstacleColor, true);
	}

	for (auto& PassRect : PassableRectSet)
	{
		m_pVisualization->drawRectangle(PassRect->m_LeftUp, PassRect->m_RightDown, PassableColor, false);
	}
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__drawCrowd()
{
	float AgentRadius = CAgentConfig::getInstance()->getAttribute<float>(AGENT_CONFIG_KEYWORD::RADIUS);
	glm::vec3 AgentColor = CColorConfig::getInstance()->getAgentColor();
	glm::vec3 GroupColor = CColorConfig::getInstance()->getGroupColor();

	std::vector<hiveCrowdSimulation::CDerivedAgent*> NoGroupAgentSet;
	m_pCrowdSimulation->dumpNoGroupAgent(NoGroupAgentSet);
	__drawNoGroupAgents(NoGroupAgentSet, AgentColor);

	std::vector<hiveCrowdSimulation::CGroup*> GroupAgentSet;
	m_pCrowdSimulation->dumpGroupAgent(GroupAgentSet);
	__drawGroupAgents(GroupAgentSet, GroupColor);
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__initSceneInfo()
{
	std::string SceneGenMethod = CSceneConfig::getInstance()->getAttribute<std::string>(SCENE_CONFIG_KEYWORD::SCENE_GENERATE);
	if (strupr(const_cast<char*>(SceneGenMethod.c_str())) == SCENE_DIVIDE::UNIFORMGRID_DIVIDE)
	{
		m_pScene = new hiveSceneProcess::CUniformGridScene();
		int UniformGridWidth = CSceneConfig::getInstance()->getAttribute<int>(SCENE_CONFIG_KEYWORD::UNIFORM_GRID_WIDTH);
		int UniforGridHeight = CSceneConfig::getInstance()->getAttribute<int>(SCENE_CONFIG_KEYWORD::UNIFORM_GRID_HEIGHT);
		m_pScene->setParameter(UniformGridWidth, UniforGridHeight);
	}

	if (strupr(const_cast<char*>(SceneGenMethod.c_str())) == SCENE_DIVIDE::QUADTREE_DIVIDE)
	{
		m_pScene = new hiveSceneProcess::CQuadTreeScene();
		int BaseNodeDeep = CSceneConfig::getInstance()->getAttribute<int>(SCENE_CONFIG_KEYWORD::BASENODE_DEEP);
		int MinNodeDeep  = CSceneConfig::getInstance()->getAttribute<int>(SCENE_CONFIG_KEYWORD::MINNODE_DEEP);
		m_pScene->setParameter(BaseNodeDeep, MinNodeDeep);
	}

	_ASSERT(m_pScene);

	std::string BmpSceneImage = CSceneConfig::getInstance()->getAttribute<std::string>(SCENE_CONFIG_KEYWORD::SCENE_IMAGE);
	m_pScene->generateScene(strlwr(const_cast<char*>(BmpSceneImage.c_str())));
}

//********************************************************************
//FUNCTION:
void hiveCrowdSimulation::CFrameWork::__drawRoadMap(const std::vector<glm::vec2>& vRoadMapSet, const glm::vec3& vColor)
{
	if (vRoadMapSet.empty()) return;

	for (unsigned int i=0; i<vRoadMapSet.size() - 1; ++i)
	{
		m_pVisualization->drawPoint(vRoadMapSet[i], 2.0, vColor, true);
		m_pVisualization->drawLine(vRoadMapSet[i], vRoadMapSet[i+1], vColor, 1.0);
	}
}