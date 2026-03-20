stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
mr r29, r3
mr r30, r4
bl fn_8025DA88
cmpwi r3, 0x2
beq @8006587C
bge @80065884
cmpwi r3, 0x0
bge @80065874
b @80065884
@80065874
li r31, 0x2
b @80065888
@8006587C
li r31, 0x1
b @80065888
@80065884
li r31, 0x1
@80065888
lis r3, lbl_803A9F08@ha
addi r3, r3, lbl_803A9F08@l
lwz r0, 0x154(r3)
cmpwi r0, 0x2
beq @800658AC
lbz r0, 0x4(r30)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r30)
@800658AC
mr r3, r29
mr r4, r30
li r5, 0x1
bl fn_80065A48
mr r3, r31
bl fn_8025DA18
clrlwi r3, r3, 16
lha r0, 0x6(r30)
mulli r4, r3, 0x3
lis r3, lbl_802ED9F0@ha
cmpwi r0, 0xbb1
addi r3, r3, lbl_802ED9F0@l
add r3, r3, r4
beq @8006590C
bge @80065900
cmpwi r0, 0xb92
beq @8006590C
bge @80065924
cmpwi r0, 0xb73
beq @8006590C
b @80065924
@80065900
cmpwi r0, 0xbd0
beq @8006590C
b @80065924
@8006590C
lbz r0, 0x0(r3)
stb r0, 0x64(r30)
lbz r0, 0x1(r3)
stb r0, 0x65(r30)
lbz r0, 0x2(r3)
stb r0, 0x66(r30)
@80065924
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
mtlr r0
addi r1, r1, 0x20
blr
