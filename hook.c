#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/icmp.h>
#include <linux/tcp.h>
#include <asm/checksum.h>

#define IP(x)(ip_hdr(x)) //Extrae la cabezera IP
#define ICMP(x)(icmp_hdr(x))
#define checksum(x,y)(ip_fast_csum(x,y)) //Suma de verificacion de la cabezera IP

/* Esta es la funcion que atiende los paquetes que entran a la maquina */

static unsigned int gancho_in(unsigned int hooknum, struct sk_buff* skb, 
				const struct net_device *in, const struct net_device *out, 
				int (*okfn)(struct sk_buff *))
{
	struct iphdr *ip = IP(skb);				//captura de la cabezera IP
	int iplen = ip->ihl * 4;				//longitud de la cabezera IP
	unsigned char *buffer = skb->network_header + iplen;	//puntero al inicio de la capa de transporte L4
		while (buffer < skb->tail) {			//XOR de L4 hasta donde termina L5, todo el paquete menos L2 L3
			*buffer = *buffer ^ 'a';
			buffer++;
		}
	return NF_ACCEPT;
}

/*Esta es la funcion que atiende los paquetes q salen de la maquina */

static unsigned int gancho_out(unsigned int hooknum, struct sk_buff* skb,
				const struct net_device *in, const struct net_device *out,
				int (*okfn)(struct sk_buff *))
{
	struct iphdr *ip = IP(skb); 				//captura de la cabezera IP
	int iplen = ip->ihl * 4;				//longitud de la cabezera IP
	unsigned char *buffer = skb->network_header + iplen;	//puntero al inicio de la capa de transporte L4
		while (buffer < skb->tail) {			//XOR de L4 hasta donde termina L5, todo el paquete menos L2 L3
			*buffer = *buffer ^ 'a';
			buffer++;
		}
	return NF_ACCEPT;
}
/* Este gancho es antes de que entre el paquete */
static struct nf_hook_ops in = {
	.pf 	   = NFPROTO_IPV4, 		//tipo de protocolo IPv4
	.priority  = 1,				//prioridad del gancho en netfilter
	.hooknum   = NF_INET_PRE_ROUTING,	//lugar donde nos registramos en netfilter antes q entre el paquete
	.hook	   = gancho_in,			//nombre de la funcion a registrar con netfilter
};
/*Este gancho es antes de que salga el paquete */
static struct nf_hook_ops out = {
	.pf	   = NFPROTO_IPV4,		//tipo de protocolo IPv4
	.priority  = 1,				//prioridad del gancho en netfilter
	.hooknum   = NF_INET_POST_ROUTING,	//lugar donde nos registramos en netfilter antes de q salga el paquete
	.hook      = gancho_out,		//nombre de la funccion a registrar con netfilter

};
/* Esta es la funcion que inicia el modulo */
static int __init inicio(void)
{
	int ret = nf_register_hook(&in); //Esta es la funcion que registrar el gancho con el subsistema netfilter
	int ret1 = nf_register_hook(&out);
	if(ret && ret1)
	{
		printk(KERN_INFO "Error no se puede iniciar el gancho");
		return ret;
	}
	else	
		printk(KERN_INFO "Bienbenidos a Netfilter");
		return ret;

}
/*Esta es la funcion que desregistra el modulo */
static void __exit salida(void)
{
	nf_unregister_hook(&in);		//Esta es la funcion que desregistra el gancho con el subsistema netfilter
	nf_unregister_hook(&out);
	printk(KERN_INFO "Adios Netfilter");
}	
module_init(inicio);
module_exit(salida);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("YUSDIEL RODRIGUEZ AMOROS");
