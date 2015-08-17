#ifndef RETROSHARE_MESH_DEMOC_GUI_INTERFACE_H
#define RETROSHARE_MESH_DEMOC_GUI_INTERFACE_H

/*
 * libretroshare/src/retroshare: rsmeshDemoc.h
 *
 * RetroShare C++ Interface.
 *
 * Copyright 2008-2012 by Robert Fernie.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License Version 2 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 *
 * Please report all bugs and problems to "retroshare@lunamutt.com".
 *
 */

#include <inttypes.h>
#include <string>
#include <list>
#include <retroshare/rsidentityVEG.h>

/* The Main Interface Class - for information about your Peers */
class RsMeshDemocVEG;
extern RsMeshDemocVEG *rsMeshDemocVEG;


class RsMeshDemocGroup
{
	public:
	RsGroupMetaData mMeta;
	RsMeshDemocGroup() { return; }
};

class RsMeshDemocMsg
{
	public:
	RsMeshDemocMsg(uint32_t t)
	:meshDemocType(t) { return; }

	RsMsgMetaData mMeta;
	uint32_t meshDemocType;
};

#define RSMESH_DEMOC_MSGTYPE_POST		0x0001
#define RSMESH_DEMOC_MSGTYPE_VOTE		0x0002
#define RSMESH_DEMOC_MSGTYPE_COMMENT	0x0004

#define RSMESH_DEMOC_PERIOD_YEAR		1
#define RSMESH_DEMOC_PERIOD_MONTH		2
#define RSMESH_DEMOC_PERIOD_WEEK		3
#define RSMESH_DEMOC_PERIOD_DAY		4
#define RSMESH_DEMOC_PERIOD_HOUR		5

#define RSMESH_DEMOC_VIEWMODE_LATEST	1
#define RSMESH_DEMOC_VIEWMODE_TOP		2
#define RSMESH_DEMOC_VIEWMODE_HOT		3
#define RSMESH_DEMOC_VIEWMODE_COMMENTS	4


class RsMeshDemocPost: public RsMeshDemocMsg
{
	public:
	RsMeshDemocPost(): RsMeshDemocMsg(RSMESH_DEMOC_MSGTYPE_POST) 
	{ 
		mMeta.mMsgFlags = RSMESH_DEMOC_MSGTYPE_POST;
		return; 
	}

	std::string mLink;
	std::string mNotes;
};


class RsMeshDemocVote: public RsMeshDemocMsg
{
	public:
	RsMeshDemocVote(): RsMeshDemocMsg(RSMESH_DEMOC_MSGTYPE_VOTE)
	{ 
		mMeta.mMsgFlags = RSMESH_DEMOC_MSGTYPE_VOTE;
		return; 
	}
};


class RsMeshDemocComment: public RsMeshDemocMsg
{
	public:
	RsMeshDemocComment(): RsMeshDemocMsg(RSMESH_DEMOC_MSGTYPE_COMMENT) 
	{ 
		mMeta.mMsgFlags = RSMESH_DEMOC_MSGTYPE_COMMENT;
		return; 
	}

	std::string mComment;
};


std::ostream &operator<<(std::ostream &out, const RsMeshDemocGroup &group);
std::ostream &operator<<(std::ostream &out, const RsMeshDemocPost &post);
std::ostream &operator<<(std::ostream &out, const RsMeshDemocVote &vote);
std::ostream &operator<<(std::ostream &out, const RsMeshDemocComment &comment);


class RsMeshDemocVEG: public RsTokenServiceVEG
{
	public:

	RsMeshDemocVEG()  { return; }
virtual ~RsMeshDemocVEG() { return; }

	/* Specific Service Data */
virtual bool getGroup(const uint32_t &token, RsMeshDemocGroup &group) = 0;
virtual bool getPost(const uint32_t &token, RsMeshDemocPost &post) = 0;
virtual bool getComment(const uint32_t &token, RsMeshDemocComment &comment) = 0;

virtual bool submitGroup(uint32_t &token, RsMeshDemocGroup &group, bool isNew) = 0;
virtual bool submitPost(uint32_t &token, RsMeshDemocPost &post, bool isNew) = 0;
virtual bool submitVote(uint32_t &token, RsMeshDemocVote &vote, bool isNew) = 0;
virtual bool submitComment(uint32_t &token, RsMeshDemocComment &comment, bool isNew) = 0;

	// Special Ranking Request.
virtual bool requestRanking(uint32_t &token, std::string groupId) = 0;
virtual bool getRankedPost(const uint32_t &token, RsMeshDemocPost &post) = 0;

virtual bool extractMeshDemocCache(const std::string &str, uint32_t &votes, uint32_t &comments) = 0;


	// Control Ranking Calculations.
virtual bool setViewMode(uint32_t mode) = 0;
virtual bool setViewPeriod(uint32_t period) = 0;
virtual bool setViewRange(uint32_t first, uint32_t count) = 0;

	// exposed for testing...
virtual float calcPostScore(const RsMsgMetaData &meta) = 0;

};


#endif
