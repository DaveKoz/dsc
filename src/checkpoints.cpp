// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 5000;

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        (0, uint256("0x"))
        (3, uint256("0x75af1d6b68b458debccd42c02ad98c13f6b4567c31256144fada8609089aa024"))
        (5, uint256("0x3464de21435fd55c70d489187ca0c4682dcfef06c1e66a8d002a654d79153b68"))
        (16, uint256("0xc0cd57766a77e79b1aad123af66dee17adf7d2835327dba490d8dd99447168bd"))
        (25, uint256("0xd2a1704b5f0ed58e9e4f25358dcf0fe00f4349ef6fbabcc7ecdf8335592a736b"))
        (29, uint256("0xcb53c03051eaec08592792842bbaae3196f09a53b4781fa138c1314fb4418770"))
        (32, uint256("0xf627cd85ab992861cbb571329d73cc329fac0e06e359981544207b6c34514778"))
        (56, uint256("0x2540f4840d8c7fa4ebd17f62115fdd5281362c4d7d5f5f9b180b919734c71976"))
        (95, uint256("0x026fb3731e3514ea2ed1452b58ed06c113a2137887c14ab9e1d666f497adbc43"))
        (126, uint256("0x7ecd6ddefe0f9ae9f239a361a36484bf1e4f6e40e179fc666c216ff468e2ccbe"))
        (199, uint256("0x93b1f66ec0520ae033c9fb2b1fb43a33f1818a8e2296e7f6851e09c0cac22c9b"))
        (549, uint256("0x63223a73163e535e371c7dda835b79f43aa852d408326ad205ae244829571c2e"))
        (943, uint256("0x8245b9bb8cd6fdc44da33af8852aea346e897c2dda6088f43efde1f46cfacbfe"))
        (1021, uint256("0x54698857097806121fa1c1569cc69f64a8f814fa1f35372975dd104547797ed9"))
        (1110, uint256("0x50dea074032d1962522c3b098064a37e3650d3a42a68cdaecfb519d5d5a72475"))
        (1156, uint256("0x8e0d1d7193911b1749776ed113da49b75dc5d7ffc082b2c156758c9c9abb5a1a"))
        (1357, uint256("0x65b3cb44fbde7a8cb2d71db9c0772e13df252cde4b8f3b2c59026144bc3be662"))
        (1965, uint256("0x67f9e6f45dfef070d26df045f225b5d2c62ef785341790f28f61105d010b60d6"))
        (2013, uint256("0x66bf1fc3f8b39164c4a0331e1eccee1229653c963b3df7281930aaa0897830d5"))
        (3201, uint256("0x9f555f136a6170b76d723ca2e64353249bf8d75f14ca5425a89e67b0d9e8a5a2"))
        (4021, uint256("0x413b94605b81bcaea38acd5794358bf5329878ba6a25a37b333f624f0e234b14"))
        (6432, uint256("0x568c1466358f85a36ad143c330e12b0509ae1da9d6687cd394c9019296fc6e51"))
        (8056, uint256("0xd052622072edbf58f682c1d2cb0963d7c408e761d58e21375cbcc2bd5524a31b"))
    ;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint 
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();
        if (nHeight <= pindexSync->nHeight){
            return false;
        }
        return true;
    }
}
