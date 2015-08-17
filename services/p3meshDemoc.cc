/*
 * libretroshare/src/services p3meshDemoc.cc
 *
 * MeshDemoc interface for RetroShare.
 *
 * Copyright 2012-2012 by Robert Fernie.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License Version 2.1 as published by the Free Software Foundation.
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

#include "services/p3meshDemoc.h"
#include "serialiser/rsmeshDemocitems.h"

#include <math.h>

/****
 * #define MESH_DEMOC_DEBUG 1
 ****/


RsMeshDemoc *rsMeshDemoc = NULL;

/********************************************************************************/
/******************* Startup / Tick    ******************************************/
/********************************************************************************/

p3MeshDemoc::p3MeshDemoc(RsGeneralDataService *gds, RsNetworkExchangeService *nes, RsGixs* gixs)
    :p3PostBase(gds, nes, gixs, new RsGxsMeshDemocSerialiser(), RS_SERVICE_GXS_TYPE_MESH_DEMOC), 
	RsMeshDemoc(this)
{
	return;
}


const std::string GXS_MESH_DEMOC_APP_NAME = "gxsmeshDemoc";
const uint16_t GXS_MESH_DEMOC_APP_MAJOR_VERSION  =       1;
const uint16_t GXS_MESH_DEMOC_APP_MINOR_VERSION  =       0;
const uint16_t GXS_MESH_DEMOC_MIN_MAJOR_VERSION  =       1;
const uint16_t GXS_MESH_DEMOC_MIN_MINOR_VERSION  =       0;

RsServiceInfo p3MeshDemoc::getServiceInfo()
{
        return RsServiceInfo(RS_SERVICE_GXS_TYPE_MESH_DEMOC,
                GXS_MESH_DEMOC_APP_NAME,
                GXS_MESH_DEMOC_APP_MAJOR_VERSION,
                GXS_MESH_DEMOC_APP_MINOR_VERSION,
                GXS_MESH_DEMOC_MIN_MAJOR_VERSION,
                GXS_MESH_DEMOC_MIN_MINOR_VERSION);
}

bool p3MeshDemoc::groupShareKeys(const RsGxsGroupId& groupId,const std::set<RsPeerId>& peers)
{
        RsGenExchange::shareGroupPublishKey(groupId,peers) ;
        return true ;
}

bool p3MeshDemoc::getGroupData(const uint32_t &token, std::vector<RsMeshDemocGroup> &groups)
{
	std::vector<RsGxsGrpItem*> grpData;
	bool ok = RsGenExchange::getGroupData(token, grpData);
		
	if(ok)
	{
		std::vector<RsGxsGrpItem*>::iterator vit = grpData.begin();
		
		for(; vit != grpData.end(); ++vit)
		{
			RsGxsMeshDemocGroupItem* item = dynamic_cast<RsGxsMeshDemocGroupItem*>(*vit);
			if (item)
			{
				RsMeshDemocGroup grp = item->mGroup;
				item->mGroup.mMeta = item->meta;
				grp.mMeta = item->mGroup.mMeta;
				delete item;
				groups.push_back(grp);
			}
			else
			{
				std::cerr << "Not a RsGxsMeshDemocGroupItem, deleting!" << std::endl;
				delete *vit;
			}
		}
	}
	return ok;
}

bool p3MeshDemoc::getPostData(const uint32_t &token, std::vector<RsMeshDemocPost> &msgs)
{
	GxsMsgDataMap msgData;
	bool ok = RsGenExchange::getMsgData(token, msgData);
	time_t now = time(NULL);

	if(ok)
	{
		GxsMsgDataMap::iterator mit = msgData.begin();
		
		for(; mit != msgData.end(); ++mit)
		{
			RsGxsGroupId grpId = mit->first;
			std::vector<RsGxsMsgItem*>& msgItems = mit->second;
			std::vector<RsGxsMsgItem*>::iterator vit = msgItems.begin();
		
			for(; vit != msgItems.end(); ++vit)
			{
				RsGxsMeshDemocPostItem* item = dynamic_cast<RsGxsMeshDemocPostItem*>(*vit);

				if(item)
				{
					RsMeshDemocPost msg = item->mPost;
					msg.mMeta = item->meta;
					msg.calculateScores(now);

					msgs.push_back(msg);
					delete item;
				}
				else
				{
					std::cerr << "Not a MeshDemocPostItem, deleting!" << std::endl;
					delete *vit;
				}
			}
		}
	}
		
	return ok;
}


bool p3MeshDemoc::getRelatedPosts(const uint32_t &token, std::vector<RsMeshDemocPost> &msgs)
{
	GxsMsgRelatedDataMap msgData;
	bool ok = RsGenExchange::getMsgRelatedData(token, msgData);
	time_t now = time(NULL);
			
	if(ok)
	{
		GxsMsgRelatedDataMap::iterator mit = msgData.begin();
		
		for(; mit != msgData.end(); ++mit)
		{
			std::vector<RsGxsMsgItem*>& msgItems = mit->second;
			std::vector<RsGxsMsgItem*>::iterator vit = msgItems.begin();
			
			for(; vit != msgItems.end(); ++vit)
			{
				RsGxsMeshDemocPostItem* item = dynamic_cast<RsGxsMeshDemocPostItem*>(*vit);
		
				if(item)
				{
					RsMeshDemocPost msg = item->mPost;
					msg.mMeta = item->meta;
					msg.calculateScores(now);

					msgs.push_back(msg);
					delete item;
				}
				else
				{
					std::cerr << "Not a MeshDemocPostItem, deleting!" << std::endl;
					delete *vit;
				}
			}
		}
	}
			
	return ok;
}


/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/

/* Switched from having explicit Ranking calculations to calculating the set of scores
 * on each RsMeshDemocPost item.
 *
 * TODO: move this function to be part of RsMeshDemocPost - then the GUI 
 * can reuse is as necessary.
 *
 */

bool RsMeshDemocPost::calculateScores(time_t ref_time)
{
	/* so we want to calculate all the scores for this Post. */

	PostStats stats;
	extractPostCache(mMeta.mServiceString, stats);

	mUpVotes = stats.up_votes;
	mDownVotes = stats.down_votes;
	mComments = stats.comments;
	mHaveVoted = (mMeta.mMsgStatus & GXS_SERV::GXS_MSG_STATUS_VOTE_MASK);

	time_t age_secs = ref_time - mMeta.mPublishTs;
#define MESH_DEMOC_AGESHIFT (2.0)
#define MESH_DEMOC_AGEFACTOR (3600.0)

	mTopScore = ((int) mUpVotes - (int) mDownVotes);
	if (mTopScore > 0)
	{
		// score drops with time.
		mHotScore =  mTopScore / pow(MESH_DEMOC_AGESHIFT + age_secs / MESH_DEMOC_AGEFACTOR, 1.5);
	}
	else
	{
		// gets more negative with time.
		mHotScore =  mTopScore * pow(MESH_DEMOC_AGESHIFT + age_secs / MESH_DEMOC_AGEFACTOR, 1.5);
	}
	mNewScore = -age_secs;

	return true;
}

/********************************************************************************************/
/********************************************************************************************/

bool p3MeshDemoc::createGroup(uint32_t &token, RsMeshDemocGroup &group)
{
	std::cerr << "p3MeshDemoc::createGroup()" << std::endl;

	RsGxsMeshDemocGroupItem* grpItem = new RsGxsMeshDemocGroupItem();
	grpItem->mGroup = group;
	grpItem->meta = group.mMeta;

	RsGenExchange::publishGroup(token, grpItem);
	return true;
}


bool p3MeshDemoc::updateGroup(uint32_t &token, RsMeshDemocGroup &group)
{
	std::cerr << "p3MeshDemoc::updateGroup()" << std::endl;

	RsGxsMeshDemocGroupItem* grpItem = new RsGxsMeshDemocGroupItem();
	grpItem->mGroup = group;
	grpItem->meta = group.mMeta;

	RsGenExchange::updateGroup(token, grpItem);
	return true;
}


bool p3MeshDemoc::createPost(uint32_t &token, RsMeshDemocPost &msg)
{
	std::cerr << "p3MeshDemoc::createPost() GroupId: " << msg.mMeta.mGroupId;
	std::cerr << std::endl;

	RsGxsMeshDemocPostItem* msgItem = new RsGxsMeshDemocPostItem();
	msgItem->mPost = msg;
	msgItem->meta = msg.mMeta;
	
	RsGenExchange::publishMsg(token, msgItem);
	return true;
}

/********************************************************************************************/
