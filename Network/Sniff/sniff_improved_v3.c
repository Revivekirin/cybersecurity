#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <arpa/inet.h>
#include <netinet/tcp.h> //For TCP header structure


/* Ethernet header */
struct ethheader {
  u_char  ether_dhost[6]; /* destination host address */
  u_char  ether_shost[6]; /* source host address */
  u_short ether_type;     /* protocol type (IP, ARP, RARP, etc) */
};

/* IP Header */
struct ipheader {
  unsigned char      iph_ihl:4, //IP header length
                     iph_ver:4; //IP version
  unsigned char      iph_tos; //Type of service
  unsigned short int iph_len; //IP Packet length (data + header)
  unsigned short int iph_ident; //Identification
  unsigned short int iph_flag:3, //Fragmentation flags
                     iph_offset:13; //Flags offset
  unsigned char      iph_ttl; //Time to Live
  unsigned char      iph_protocol; //Protocol type
  unsigned short int iph_chksum; //IP datagram checksum
  struct  in_addr    iph_sourceip; //Source IP address
  struct  in_addr    iph_destip;   //Destination IP address
};

void got_packet(u_char *args, const struct pcap_pkthdr *header,
                              const u_char *packet)
{
  struct ethheader *eth = (struct ethheader *)packet;

  if (ntohs(eth->ether_type) == 0x0800) { // 0x0800 is IP type
    struct ipheader * ip = (struct ipheader *)
                           (packet + sizeof(struct ethheader)); 

    //Check if it's a TCP packet
    if (ip->iph_protocol == IPPROTO_TCP) {
        //Calculate the size of the Ethernet and IP headers
        int eth_ip_header_size = sizeof(struct ethheader) + (ip->iph_ih1 <<2);

        //Get a pointer to the TCP header
        struct tcphdr *tcp = (struct tcphdr *)(packet + eth_ip_header_size);

        //Extract and print source and destination ports
        uint16_t src.port = ntohs(tcp->th_sport);
        uint16_t dst_port = ntohs(tcp->th_dport);

        printf("Source Port: %d\n", src_port);
        printf("Destination Port: %d\n", dst_port);
    }

    printf("       From: %s\n", inet_ntoa(ip->iph_sourceip));   
    printf("         To: %s\n", inet_ntoa(ip->iph_destip));    

    printf("Source MAC Address: %02x:%02x:%02x:%02x:%02x:%02x:\n", 
            eth-> ether_shost[0], eth->ether_shost[1], eth->ether_shost[2],
            eth-> ether_shost[3], eth-> ether_shost[4], eth-> ether_shost[5]);

    printf("Dest MAC Address: %02x:%02x:%02x:%02x:%02x:%02x:\n", 
            eth-> ether_dhost[0], eth->ether_dhost[1], eth->ether_dhost[2],
            eth-> ether_dhost[3], eth-> ether_dhost[4], eth-> ether_dhost[5]);



    /* determine protocol */
    switch(ip->iph_protocol) {                                 
        case IPPROTO_TCP:
            printf("   Protocol: TCP\n");
            return;
        case IPPROTO_UDP:
            printf("   Protocol: UDP\n");
            return;
        case IPPROTO_ICMP:
            printf("   Protocol: ICMP\n");
            return;
        default:
            printf("   Protocol: others\n");
            return;
    }
  }
}

int main()
{
  pcap_t *handle;
  char errbuf[PCAP_ERRBUF_SIZE];
  struct bpf_program fp;
  char filter_exp[] = "icmp";
  bpf_u_int32 net;

  // Step 1: Open live pcap session on NIC with name enp0s3
  handle = pcap_open_live("enp0s3", BUFSIZ, 1, 1000, errbuf);

  // Step 2: Compile filter_exp into BPF psuedo-code
  pcap_compile(handle, &fp, filter_exp, 0, net);
  if (pcap_setfilter(handle, &fp) !=0) {
      pcap_perror(handle, "Error:");
      exit(EXIT_FAILURE);
  }

  // Step 3: Capture packets
  pcap_loop(handle, -1, got_packet, NULL);

  pcap_close(handle);   //Close the handle
  return 0;
}


