lis r3, lbl_803A9A60@ha
addi r3, r3, lbl_803A9A60@l
lwz r0, 0x4(r3)
cmpwi r0, 0x1
beq @80061BA8
bgelr
cmpwi r0, 0x0
bltlr
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
blr
@80061BA8
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
blr
