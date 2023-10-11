#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>


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
/*the packet argument contains a copy of the packet, including the Ethernet header.
We typecast it to the Ethernet header structure.*/

{
  struct ethheader *eth = (struct ethheader *)packet;

  struct tcphdr *tcp_header;

  //Assuming Ethernet frame with IP and TCP headers
  struct ip *ip = (struct ip *)(packet +14); //skip Ethernet header
  tcp_header = (struct tcphdr *)(packet +14 + (ip->ip_h1 <<2)); //skip Ethernet and IP headers

  //Extract and prind source and destination ports
  uint16_t src.port = ntohs(tcp_header->th_sport);
  uint16_t dst_port = ntohs(tcp_header -> th_dport);

  printf("Source Port: %d\n", src_port);
  printf("Destination Port : %d\n", dst_port)
   
   /*now we can access the field of the structure*/
  if (ntohs(eth->ether_type) == 0x0800) { // 0x0800 is IP type
    struct ipheader * ip = (struct ipheader *)
                           (packet + sizeof(struct ethheader));  /*Find where the IP header starts, and typecast it to the IP Header structure*/

    printf("       From: %s\n", inet_ntoa(ip->iph_sourceip));   /*Now we can easily access the fileds in the IP header.*/
    printf("         To: %s\n", inet_ntoa(ip->iph_destip));    
    
    

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
  char filter_exp[] = "tcp";
  bpf_u_int32 net;

  /*Initialize a raw socket, set the network device into promiscuous mode.*/
  // Step 1: Open live pcap session on NIC with name enp0s3
  handle = pcap_open_live("enp0s3", BUFSIZ, 1, 1000, errbuf); 

  // Step 2: Compile filter_exp into BPF psuedo-code
  pcap_compile(handle, &fp, filter_exp, 0, net); /* char filter_exp[] = "icmp";*/
  if (pcap_setfilter(handle, &fp) !=0) {
      pcap_perror(handle, "Error:");
      exit(EXIT_FAILURE);
  }

  // Step 3: Capture packets
  pcap_loop(handle, -1, got_packet, NULL);

  pcap_close(handle);   //Close the handle
  return 0;
}


