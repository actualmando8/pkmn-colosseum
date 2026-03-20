stwu r1, -0x50(r1)
mflr r0
stw r0, 0x54(r1)
stw r31, 0x4c(r1)
stw r30, 0x48(r1)
mr r31, r4
lis r4, lbl_8026F5C0@ha
li r3, 0x53
addi r12, r4, lbl_8026F5C0@l
lwz r11, 0x0(r12)
lwz r10, 0x4(r12)
lwz r9, 0x8(r12)
lwz r8, 0xc(r12)
lwz r7, 0x10(r12)
lwz r6, 0x14(r12)
lwz r5, 0x18(r12)
lwz r4, 0x1c(r12)
lwz r0, 0x20(r12)
stw r11, 0x20(r1)
stw r10, 0x24(r1)
stw r9, 0x28(r1)
stw r8, 0x14(r1)
stw r7, 0x18(r1)
stw r6, 0x1c(r1)
stw r5, 0x8(r1)
stw r4, 0xc(r1)
stw r0, 0x10(r1)
bl fn_80104704
cmplwi r3, 0x0
beq @80096D3C
lbz r0, 0x95(r3)
extsb r0, r0
cmpwi r0, 0x1
beq @80096CF4
bge @80096CE0
cmpwi r0, 0x0
bge @80096CEC
b @80096D00
@80096CE0
cmpwi r0, 0x3
bge @80096D00
b @80096CFC
@80096CEC
addi r30, r1, 0x20
b @80096D00
@80096CF4
addi r30, r1, 0x14
b @80096D00
@80096CFC
addi r30, r1, 0x8
@80096D00
lfs f0, 0x0(r30)
fctiwz f0, f0
stfd f0, 0x30(r1)
lwz r0, 0x34(r1)
stb r0, 0x64(r31)
lfs f0, 0x4(r30)
fctiwz f0, f0
stfd f0, 0x38(r1)
lwz r0, 0x3c(r1)
stb r0, 0x65(r31)
lfs f0, 0x8(r30)
fctiwz f0, f0
stfd f0, 0x40(r1)
lwz r0, 0x44(r1)
stb r0, 0x66(r31)
@80096D3C
lwz r0, 0x54(r1)
lwz r31, 0x4c(r1)
lwz r30, 0x48(r1)
mtlr r0
addi r1, r1, 0x50
blr
