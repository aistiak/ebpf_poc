#include <linux/bpf.h>
#include <linux/pkt_cls.h>
#include <linux/types.h>

SEC("classifier")
int classifier(struct __sk_buff *skb) {
    // Define packet data pointer and end pointer
    void *data = (void *)(long)skb->data;
    void *data_end = (void *)(long)skb->data_end;

    // Define pointers to the Ethernet, IP, and TCP headers
    struct ethhdr *eth = data;
    struct iphdr *ip = data + sizeof(struct ethhdr);
    struct tcphdr *tcp = data + sizeof(struct ethhdr) + sizeof(struct iphdr);

    // Check if packet contains at least Ethernet, IP, and TCP headers
    if (data + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct tcphdr) > data_end)
        return TC_ACT_OK;

    // Ensure packet is TCP and destined for port 80 (HTTP)
    if (eth->h_proto != htons(ETH_P_IP) || ip->protocol != IPPROTO_TCP || ntohs(tcp->dest) != 80)
        return TC_ACT_OK;

    // Define pointer to TCP payload (HTTP data)
    void *http_data = (void *)(long)skb->data + sizeof(struct ethhdr) + sizeof(struct iphdr) + tcp->doff * 4;

    // Define pointers to the start and end of the TCP payload
    void *ptr = http_data;
    void *end = data_end;

    // Flag to indicate if "crash" header is found and set to "1"
    int crash_header_found = 0;

    // Check for the "crash" header in the HTTP request
    while (ptr + 5 < end) {
        // Check if the current position contains the "crash" header
        if (ptr + 6 < end && *((char *)ptr) == 'C' && *((char *)ptr + 1) == 'r' && *((char *)ptr + 2) == 'a' && *((char *)ptr + 3) == 's' && *((char *)ptr + 4) == 'h' && *((char *)ptr + 5) == ':' && *((char *)ptr + 7) == '1') {
            // Set the flag indicating that the "crash" header is found and set to "1"
            crash_header_found = 1;
            break;
        }
        ptr++;
    }

    // If "crash" header is found and set to "1", drop the packet
    if (crash_header_found) {
        return TC_ACT_SHOT;
    }

    return TC_ACT_OK;
}

char __license[] SEC("license") = "GPL";
