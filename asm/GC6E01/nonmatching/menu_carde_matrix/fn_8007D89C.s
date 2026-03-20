stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
mr r31, r4
cmplwi r3, 0x0
bne @8007D8C0
li r3, 0xa6
bl fn_80104704
@8007D8C0
bl fn_801040A0
lwz r3, 0x0(r3)
cmplwi r3, 0x0
beq @8007D964
lha r0, 0x6(r31)
cmpwi r0, 0x791
bne @8007D910
lwz r0, 0xac(r3)
cmpwi r0, 0x0
ble @8007D8F4
lwz r0, 0xa0(r3)
cmpwi r0, 0x0
bge @8007D8FC
@8007D8F4
li r0, 0x0
b @8007D908
@8007D8FC
lwz r3, 0xb0(r3)
slwi r0, r0, 2
lwzx r0, r3, r0
@8007D908
mr r4, r0
b @8007D940
@8007D910
lwz r0, 0xac(r3)
cmpwi r0, 0x0
ble @8007D928
lwz r0, 0xa4(r3)
cmpwi r0, 0x0
bge @8007D930
@8007D928
li r0, 0x0
b @8007D93C
@8007D930
lwz r3, 0xb0(r3)
slwi r0, r0, 2
lwzx r0, r3, r0
@8007D93C
mr r4, r0
@8007D940
cmplwi r4, 0x0
beq @8007D95C
li r0, 0xe3
li r3, 0x37
stw r0, 0x4c(r31)
bl fn_80132A38
b @8007D964
@8007D95C
li r0, 0x0
stw r0, 0x4c(r31)
@8007D964
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
