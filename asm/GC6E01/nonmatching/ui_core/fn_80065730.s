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
beq @80065774
bge @8006577C
cmpwi r3, 0x0
bge @8006576C
b @8006577C
@8006576C
li r31, 0x1
b @80065780
@80065774
li r31, 0x2
b @80065780
@8006577C
li r31, 0x2
@80065780
lis r3, lbl_803A9F08@ha
addi r3, r3, lbl_803A9F08@l
lwz r0, 0x154(r3)
cmpwi r0, 0x2
beq @800657A4
lbz r0, 0x4(r30)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r30)
@800657A4
mr r3, r29
mr r4, r30
li r5, 0x2
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
beq @80065804
bge @800657F8
cmpwi r0, 0xb92
beq @80065804
bge @8006581C
cmpwi r0, 0xb73
beq @80065804
b @8006581C
@800657F8
cmpwi r0, 0xbd0
beq @80065804
b @8006581C
@80065804
lbz r0, 0x0(r3)
stb r0, 0x64(r30)
lbz r0, 0x1(r3)
stb r0, 0x65(r30)
lbz r0, 0x2(r3)
stb r0, 0x66(r30)
@8006581C
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
mtlr r0
addi r1, r1, 0x20
blr
