--- a/drivers/net/ethernet/atheros/atl1e/atl1e_main.c
+++ b/drivers/net/ethernet/atheros/atl1e/atl1e_main.c
@@ -375,6 +375,7 @@
 	return 0;
 }
 
+#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,39))
 static netdev_features_t atl1e_fix_features(struct net_device *netdev,
 	netdev_features_t features)
 {
@@ -400,6 +401,7 @@
 
 	return 0;
 }
+#endif /* (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,39)) */
 
 /**
  * atl1e_change_mtu - Change the Maximum Transfer Unit
@@ -1925,7 +1927,11 @@
 	 * reschedule our watchdog timer */
 	set_bit(__AT_DOWN, &adapter->flags);
 
+#if defined(NETIF_F_LLTX) || (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,39))
 	netif_stop_queue(netdev);
+#else
+	netif_tx_disable(netdev);
+#endif
 
 	/* reset MAC to disable all RX/TX */
 	atl1e_reset_hw(&adapter->hw);
@@ -2195,8 +2201,10 @@
 	.ndo_set_rx_mode	= atl1e_set_multi,
 	.ndo_validate_addr	= eth_validate_addr,
 	.ndo_set_mac_address	= atl1e_set_mac_addr,
+#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,39))
 	.ndo_fix_features	= atl1e_fix_features,
 	.ndo_set_features	= atl1e_set_features,
+#endif /* (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,39)) */
 	.ndo_change_mtu		= atl1e_change_mtu,
 	.ndo_do_ioctl		= atl1e_ioctl,
 	.ndo_tx_timeout		= atl1e_tx_timeout,
@@ -2216,10 +2224,15 @@
 	netdev->watchdog_timeo = AT_TX_WATCHDOG;
 	atl1e_set_ethtool_ops(netdev);
 
+#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,39))
 	netdev->hw_features = NETIF_F_SG | NETIF_F_HW_CSUM | NETIF_F_TSO |
 			      NETIF_F_HW_VLAN_RX;
 	netdev->features = netdev->hw_features | NETIF_F_LLTX |
 			   NETIF_F_HW_VLAN_TX;
+#else
+	netdev->features = NETIF_F_SG | NETIF_F_HW_CSUM | NETIF_F_TSO |
+			   NETIF_F_HW_VLAN_RX | NETIF_F_LLTX | NETIF_F_HW_VLAN_TX;
+#endif /* (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,39)) */
 
 	return 0;
 }