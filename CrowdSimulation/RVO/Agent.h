/*
 * Agent.h
 * RVO2 Library
 *
 * Copyright (c) 2008-2010 University of North Carolina at Chapel Hill.
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for educational, research, and non-profit purposes, without
 * fee, and without a written agreement is hereby granted, provided that the
 * above copyright notice, this paragraph, and the following four paragraphs
 * appear in all copies.
 *
 * Permission to incorporate this software into commercial products may be
 * obtained by contacting the Office of Technology Development at the University
 * of North Carolina at Chapel Hill <otd@unc.edu>.
 *
 * This software program and documentation are copyrighted by the University of
 * North Carolina at Chapel Hill. The software program and documentation are
 * supplied "as is," without any accompanying services from the University of
 * North Carolina at Chapel Hill or the authors. The University of North
 * Carolina at Chapel Hill and the authors do not warrant that the operation of
 * the program will be uninterrupted or error-free. The end-user understands
 * that the program was developed for research purposes and is advised not to
 * rely exclusively on the program for any reason.
 *
 * IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE
 * AUTHORS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS
 * SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF NORTH CAROLINA AT
 * CHAPEL HILL OR THE AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS SPECIFICALLY
 * DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND ANY
 * STATUTORY WARRANTY OF NON-INFRINGEMENT. THE SOFTWARE PROVIDED HEREUNDER IS ON
 * AN "AS IS" BASIS, AND THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE
 * AUTHORS HAVE NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 * ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Please send all bug reports to <geom@cs.unc.edu>.
 *
 * The authors may be contacted via:
 *
 * Jur van den Berg, Stephen J. Guy, Jamie Snape, Ming C. Lin, Dinesh Manocha
 * Dept. of Computer Science
 * 201 S. Columbia St.
 * Frederick P. Brooks, Jr. Computer Science Bldg.
 * Chapel Hill, N.C. 27599-3175
 * United States of America
 *
 * <http://gamma.cs.unc.edu/RVO2/>
 */

#ifndef RVO_AGENT_H_
#define RVO_AGENT_H_

/**
 * \file       Agent.h
 * \brief      Contains the Agent class.
 */

#include "Definitions.h"
#include "RVOSimulator.h"
#include "RVOExport.h"

#include <boost/serialization/serialization.hpp>

namespace RVO 
{
	enum EGroupMemberState
	{
		GMS_WAITING_MODE = 0,
		GMS_WALKING_MODE
	};

	class RVO__DLL_EXPORT Agent 
	{
	public:
		explicit Agent(RVOSimulator *sim);
		Agent(RVOSimulator *sim, const glm::vec2 &position);

		/**
		 * \brief      Constructs an agent instance with the specified
		 *             properties.
		 * \param      sim             The simulator instance.
		 * \param      position        The two-dimensional starting position of
		 *                             this agent.
		 * \param      neighborDist    The maximum neighbor distance of this agent.
		 * \param      maxNeighbors    The maximum neighbor count of this agent.
		 * \param      radius          The radius of this agent.
		 * \param      timeHorizon     The time horizon of this agent.
		 * \param      timeHorizonObst The time horizon of this agent with respect
		 *                             to static obstacles.
		 * \param      maxSpeed        The maximum speed of this agent.
		 * \param      velocity        The initial two-dimensional linear velocity
		 *                             of this agent.
		 */
		Agent(RVOSimulator *sim, const glm::vec2 &position, float neighborDist,
			  size_t maxNeighbors, float timeHorizon, float timeHorizonObst,
			  float radius, const glm::vec2 &velocity, float maxSpeed);

		~Agent() {}
		/**
		 * \brief      Computes the neighbors of this agent.
		 */
		void computeNeighbors();

		/**
		 * \brief      Computes the new velocity of this agent.
		 */
		void computeNewVelocity();

		/**
		 * \brief      Inserts an agent neighbor into the set of neighbors of
		 *             this agent.
		 * \param      agentNo         The number of the agent to be inserted.
		 * \param      rangeSq         The squared range around this agent.
		 */
		void insertAgentNeighbor(size_t agentNo, float &rangeSq);

		/**
		 * \brief      Inserts a static obstacle neighbor into the set of neighbors
		 *             of this agent.
		 * \param      obstacleNo      The number of the static obstacle to be
		 *                             inserted.
		 * \param      rangeSq         The squared range around this agent.
		 */
		void insertObstacleNeighbor(size_t obstacleNo, float rangeSq);

		/**
		 * \brief      Updates the two-dimensional position and two-dimensional
		 *             velocity of this agent.
		 */
		void update();
		bool judgePointInRectangle(const glm::vec2& Point, const std::vector<glm::vec2>& Rectangle);
		int judgeCollisionDirection(const glm::vec2& Point, const std::vector<glm::vec2>& Rectangle);
		void adjustPrefVelocity();

	public:
		const glm::vec2& getPosition() const {return position_;}
		const glm::vec2& getGoalPos() const {return goalPos;}
		const glm::vec2& getVelocity() const {return velocity_;} 
		const glm::vec2& getPrefVelocity() const {return prefVelocity_;}
		float getMaxSpeed() const {return maxSpeed_;}
		float getRadius() const {return radius_;}
		float getNeighborDist() const {return neighborDist_;}
		//float getDistanceToGoal() const {return distanceToGoal;}
		size_t getMaxNeighbors() const {return maxNeighbors_;}
		int getGroupID() const {return GroupID;}

		void setNeighborDist(float vNeighborDis) {neighborDist_ = vNeighborDis;}
		void setVelocity(const glm::vec2& vVelocity) {velocity_ = vVelocity;}
		void setMaxNeighbors(size_t vNeighbors) {maxNeighbors_ = vNeighbors;}
		void setMaxSpeed(float vMaxSpeed) {maxSpeed_ = vMaxSpeed;}
		void setRadius(float vRadius) {radius_ = vRadius;} 
		void setPosition(const glm::vec2& vPos) {position_ = vPos;}
		//void setDistanceToGoal(float vDistance) {distanceToGoal = vDistance;}
		void setGoalPos(const glm::vec2 vGoalPos) {goalPos = vGoalPos; /*setDistanceToGoal(FLT_MAX);*/}
		void setGroupID(int vID) {_ASSERT(vID >= 0); GroupID = vID;}
		void setPrefVelocity(const glm::vec2& Velocity) {prefVelocity_ = Velocity;}

		void changeToWaitingMode() {m_State = EGroupMemberState::GMS_WAITING_MODE;}
		void changeToWalkingMode() {m_State = EGroupMemberState::GMS_WALKING_MODE;}

		//bool isFinished(float vRadiusOfGoal);
		//void setTofinished() {m_IsReached = true;}
		bool isWaitingState() {return m_State == EGroupMemberState::GMS_WAITING_MODE;}
		bool isWalkingState() {return m_State == EGroupMemberState::GMS_WALKING_MODE;}

		//add new method: 2017/05/04
		void setMoveSpeed(float vSpeed) {m_MoveSpeed = vSpeed;}
		float getMoveSpeed() const      {return m_MoveSpeed;}

		void setIgnorance() {m_IsAwareness = false;}
		bool isAwareness() const {return m_IsAwareness;}

	private:
		//bool m_IsReached;
		size_t maxNeighbors_;
		float maxSpeed_;
		float neighborDist_;
		glm::vec2 newVelocity_;
		glm::vec2 position_;
		glm::vec2 prefVelocity_;
		float radius_;
		RVOSimulator *sim_;
		float timeHorizon_;
		float timeHorizonObst_;
		//float distanceToGoal;
		glm::vec2 velocity_;
		glm::vec2 goalPos;
		size_t GroupID;
		size_t m_ContinuousCollideNum;
		std::vector<Line> orcaLines_;
		std::vector<std::pair<float, size_t> > agentNeighbors_;
		std::vector<std::pair<float, size_t> > obstacleNeighbors_;
		int m_CollisitionDirection;

		EGroupMemberState m_State;

		//add new attributes: 2017/05/04
		float m_MoveSpeed;
		bool  m_IsAwareness;

		friend class KdTree;
		friend class RVOSimulator;
		//friend class CAgent;

		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & maxNeighbors_;
			ar & maxSpeed_;
			ar & neighborDist_;
			ar & newVelocity_.x;
			ar & newVelocity_.y;
			ar & position_.x;
			ar & position_.y;
			ar & prefVelocity_.x;
			ar & prefVelocity_.y;
			ar & radius_;
		}

	};

	/**
	 * \relates    Agent
	 * \brief      Solves a one-dimensional linear program on a specified line
	 *             subject to linear constraints defined by lines and a circular
	 *             constraint.
	 * \param      lines         Lines defining the linear constraints.
	 * \param      lineNo        The specified line constraint.
	 * \param      radius        The radius of the circular constraint.
	 * \param      optVelocity   The optimization velocity.
	 * \param      directionOpt  True if the direction should be optimized.
	 * \param      result        A reference to the result of the linear program.
	 * \return     True if successful.
	 */
	bool linearProgram1(const std::vector<Line> &lines, size_t lineNo,
						float radius, const glm::vec2 &optVelocity,
						bool directionOpt, glm::vec2 &result);

	/**
	 * \relates    Agent
	 * \brief      Solves a two-dimensional linear program subject to linear
	 *             constraints defined by lines and a circular constraint.
	 * \param      lines         Lines defining the linear constraints.
	 * \param      num           Count of lines.
	 * \param      radius        The radius of the circular constraint.
	 * \param      optVelocity   The optimization velocity.
	 * \param      directionOpt  True if the direction should be optimized.
	 * \param      result        A reference to the result of the linear program.
	 * \return     True if successful.
	 */
	bool linearProgram2(const std::vector<Line> &lines, size_t num, float radius,
						const glm::vec2 &optVelocity, bool directionOpt,
						glm::vec2 &result);

	/**
	 * \relates    Agent
	 * \brief      Solves a two-dimensional linear program subject to linear
	 *             constraints defined by lines and a circular constraint.
	 * \param      lines         Lines defining the linear constraints.
	 * \param      numObstLines  Count of obstacle lines.
	 * \param      radius        The radius of the circular constraint.
	 * \param      result        A reference to the result of the linear program.
	 */
	void linearProgram3(const std::vector<Line> &lines, size_t numObstLines,
						float radius, glm::vec2 &result);


	//float computeAngle(const glm::vec2& vVectorA, const glm::vec2& vVectorB);
}

#endif /* RVO_AGENT_H_ */
