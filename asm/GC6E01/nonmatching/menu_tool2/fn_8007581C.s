stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
stw r30, 0x8(r1)
li r30, 0x1
li r31, 0x0
b @80075978
@8007583C
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_8005E750
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_80063D14
mr r31, r3
cmpwi r31, 0xb8
beq @80075880
bge @80075880
cmpwi r31, 0xb3
beq @80075878
b @80075880
@80075878
bl fn_80071398
b @80075984
@80075880
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_80069A60
mr r31, r3
li r3, 0x0
li r4, 0xe
bl fn_80129280
stw r31, 0x20(r3)
li r3, 0x0
li r4, 0xe
bl fn_80129280
bl fn_80062948
mr r31, r3
cmpwi r31, 0xb4
beq @8007596C
bge @800758E4
cmpwi r31, 0xac
beq @8007594C
bge @800758DC
cmpwi r31, -0x1
beq @8007596C
b @8007596C
@800758DC
cmpwi r31, 0xb3
b @8007596C
@800758E4
cmpwi r31, 0xd1
beq @80075904
bge @800758F8
cmpwi r31, 0xb6
b @8007596C
@800758F8
cmpwi r31, 0x105
beq @8007593C
b @8007596C
@80075904
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x0(r3)
cmpwi r0, 0x1
bne @80075978
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x20(r3)
cmpwi r0, 0x2
bne @80075978
li r30, 0x0
b @80075978
@8007593C
li r3, 0x105
bl fn_800715BC
li r30, 0x0
b @80075978
@8007594C
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x0(r3)
cmpwi r0, 0x0
beq @80075968
b @8007596C
@80075968
li r31, 0xae
@8007596C
mr r3, r31
bl fn_80071398
li r30, 0x0
@80075978
clrlwi r0, r30, 24
cmplwi r0, 0x0
bne @8007583C
@80075984
bl fn_800FF52C
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80075A14
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x0(r3)
cmpwi r0, 0x1
beq @800759C4
lis r3, lbl_802688F8@ha
lis r5, lbl_8026890C@ha
addi r3, r3, lbl_802688F8@l
li r4, 0xa7
addi r5, r5, lbl_8026890C@l
bl fn_80196E10
@800759C4
bl fn_800FF660
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x0(r3)
cmpwi r0, 0x1
beq @800759E4
b @80075A1C
@800759E4
cmpwi r31, 0xd1
beq @80075A1C
li r3, 0x8ae
li r4, 0x0
bl fn_8019075C
lis r3, 0x596
li r4, 0x0
addi r3, r3, 0x9
bl fn_8011288C
li r3, 0x1
bl fn_801C40F0
b @80075A1C
@80075A14
li r3, 0x395
bl fn_800FF58C
@80075A1C
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
lwz r30, 0x8(r1)
mtlr r0
addi r1, r1, 0x10
blr
