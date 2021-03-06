#ifndef RSMESH_DEMOCITEMS_H
#define RSMESH_DEMOCITEMS_H

#include "serialiser/rsserviceids.h"
#include "serialiser/rsserial.h"
//#include "serialiser/rstlvtypes.h"

#include "serialiser/rsgxscommentitems.h"

#include "serialiser/rsgxsitems.h"
#include "interface/rsmeshDemoc.h"

const uint16_t RS_SERVICE_GXS_TYPE_MESH_DEMOC      = 0x0216;

const uint8_t RS_PKT_SUBTYPE_MESH_DEMOC_GRP_ITEM = 0x02;
const uint8_t RS_PKT_SUBTYPE_MESH_DEMOC_POST_ITEM = 0x03;

class RsGxsMeshDemocGroupItem : public RsGxsGrpItem
{
public:
	RsGxsMeshDemocGroupItem() : RsGxsGrpItem(RS_SERVICE_GXS_TYPE_MESH_DEMOC,
		 RS_PKT_SUBTYPE_MESH_DEMOC_GRP_ITEM) { return; }
	virtual ~RsGxsMeshDemocGroupItem() { return; }

	void clear();
	std::ostream &print(std::ostream &out, uint16_t indent = 0);

	RsMeshDemocGroup mGroup;
};

class RsGxsMeshDemocPostItem : public RsGxsMsgItem
{
public:
	RsGxsMeshDemocPostItem() : RsGxsMsgItem(RS_SERVICE_GXS_TYPE_MESH_DEMOC,
		 RS_PKT_SUBTYPE_MESH_DEMOC_POST_ITEM) {return ; }
	virtual ~RsGxsMeshDemocPostItem() { return; }

	void clear();
	std::ostream &print(std::ostream &out, uint16_t indent = 0);

	RsMeshDemocPost mPost;
};

class RsGxsMeshDemocSerialiser : public RsGxsCommentSerialiser
{

public:

	RsGxsMeshDemocSerialiser()
	:RsGxsCommentSerialiser(RS_SERVICE_GXS_TYPE_MESH_DEMOC)
	{ return; }

	virtual ~RsGxsMeshDemocSerialiser() { return; }

	uint32_t size(RsItem *item);
	bool serialise(RsItem *item, void *data, uint32_t *size);
	RsItem* deserialise(void *data, uint32_t *size);

private:

	uint32_t sizeGxsMeshDemocGroupItem(RsGxsMeshDemocGroupItem* item);
	bool serialiseGxsMeshDemocGroupItem(RsGxsMeshDemocGroupItem* item, void* data, uint32_t *size);
	RsGxsMeshDemocGroupItem* deserialiseGxsMeshDemocGroupItem(void *data, uint32_t *size);

	uint32_t sizeGxsMeshDemocPostItem(RsGxsMeshDemocPostItem* item);
	bool serialiseGxsMeshDemocPostItem(RsGxsMeshDemocPostItem* item, void* data, uint32_t *size);
	RsGxsMeshDemocPostItem* deserialiseGxsMeshDemocPostItem(void *data, uint32_t *size);
};


#endif // RSMESH_DEMOCITEMS_H
