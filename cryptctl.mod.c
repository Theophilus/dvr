#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x92bb2bf4, "module_layout" },
	{ 0xbda0bf93, "cdev_del" },
	{ 0x25827ecd, "__class_create" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0xe914e41e, "strcpy" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0xf3248a6e, "class_destroy" },
	{ 0x714e6dc0, "device_destroy" },
	{ 0x5f8eb667, "cdev_add" },
	{ 0x83f7b822, "cdev_init" },
	{ 0xe1aa08c3, "device_create" },
	{ 0x91715312, "sprintf" },
	{ 0x27e1a049, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "2B084C1AD3B932BD9E0A720");
