void PeerManagerImpl::InitializeNode(CNode *pnode)
{
    NodeId nodeid = pnode->GetId();
    {
        LOCK(cs_main);
        mapNodeState.emplace_hint(mapNodeState.end(), std::piecewise_construct, std::forward_as_tuple(nodeid), std::forward_as_tuple(pnode->IsInboundConn()));
        assert(m_txrequest.Count(nodeid) == 0);
    }
    PeerRef peer = std::make_shared<Peer>(nodeid, /*tx_relay=*/ !pnode->IsBlockOnlyConn());
    {
        LOCK(m_peer_mutex);
        m_peer_map.emplace_hint(m_peer_map.end(), nodeid, peer);
    }
    if (!pnode->IsInboundConn()) {
        PushNodeVersion(*pnode, *peer);
    }
}
