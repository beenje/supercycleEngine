#include <stdio.h>
//#include <subRecord.h>
#include <aSubRecord.h>
#include <registryFunction.h>
#include <epicsExport.h>

#include "engine.hpp"
#include "ioblock.hpp"
#include "cmnbase.hpp"
#include "iocVars.hpp"
#include "ioblock.hpp"
#include "devStringoutCtrl.hpp"

static long initEngine(aSubRecord *prec)
{
    iocVars2IO();
    return 0;
}

static long ioEngine(aSubRecord *prec)
{
    static io::IOBlock &io_block = io::RegisteredIOBlock();

    io_block.dbSync(RegisteredStrOutMap);

    engineCycle(io_block);
    // Update the meta
    epicsUInt64 *pvalaU64 = (epicsUInt64 *)prec->vala;
    epicsUInt32 *pvalaU32 = (epicsUInt32 *)prec->vala;

    pvalaU64[0] = io_block.cId; // 0,1
    pvalaU32[2] = (epicsUInt32)io_block.cPeriod;
    pvalaU32[3] = (epicsUInt32)env::CSYNC;

    // Update the Dbuf
    // neva , novb (max)
    prec->nevb = cmn::vec2p<epicsUInt32>(prec->valb, io_block.dbuf.vallist());
    prec->nevc = cmn::vec2p<epicsUInt32>(prec->valc, io_block.Seq.getSeqTst());
    prec->nevd = cmn::vec2p<epicsUInt32>(prec->vald, io_block.Seq.getSeqEvt());

    epicsUInt32 *paU32 = (epicsUInt32 *)prec->a;
    io_block.cOffset = paU32[0];

    return 0;
}

// Register the function

epicsRegisterFunction(initEngine);
epicsRegisterFunction(ioEngine);
