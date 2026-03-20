stwu r1, -0x90(r1)
mflr r0
stw r0, 0x94(r1)
stw r31, 0x8c(r1)
lis r3, lbl_803FAEF8@ha
li r4, 0x0
addi r6, r3, lbl_803FAEF8@l
@8007B130
mr r5, r4
li r3, 0x8
li r0, 0x2
mtctr r0
@8007B140
clrlwi r0, r5, 31
cmplwi r0, 0x0
beq @8007B15C
srwi r0, r5, 1
xoris r5, r0, 0xedb8
xori r5, r5, 0x8320
b @8007B160
@8007B15C
srwi r5, r5, 1
@8007B160
clrlwi r0, r5, 31
cmplwi r0, 0x0
beq @8007B17C
srwi r0, r5, 1
xoris r5, r0, 0xedb8
xori r5, r5, 0x8320
b @8007B180
@8007B17C
srwi r5, r5, 1
@8007B180
clrlwi r0, r5, 31
cmplwi r0, 0x0
beq @8007B19C
srwi r0, r5, 1
xoris r5, r0, 0xedb8
xori r5, r5, 0x8320
b @8007B1A0
@8007B19C
srwi r5, r5, 1
@8007B1A0
clrlwi r0, r5, 31
cmplwi r0, 0x0
beq @8007B1BC
srwi r0, r5, 1
xoris r5, r0, 0xedb8
xori r5, r5, 0x8320
b @8007B1C0
@8007B1BC
srwi r5, r5, 1
@8007B1C0
subi r3, r3, 0x3
bdnz @8007B140
stw r5, 0x0(r6)
addi r6, r6, 0x4
addi r4, r4, 0x1
cmpwi r4, 0x100
blt @8007B130
li r0, 0x0
stw r0, lbl_8047A64C@sda21(r0)
bl OSGetTick
lis r5, 0xaaab
lis r4, lbl_802EE608@ha
subi r0, r5, 0x5555
lis r5, lbl_803FADF8@ha
mulhwu r6, r0, r3
addi r4, r4, lbl_802EE608@l
addi r0, r5, lbl_803FADF8@l
srwi r6, r6, 1
mulli r5, r6, 0x3
subf r5, r5, r3
mr r3, r0
slwi r0, r5, 2
lwzx r4, r4, r0
bl fn_800CA968
lis r3, lbl_803FADF8@ha
addi r4, r1, 0x44
addi r3, r3, lbl_803FADF8@l
bl fn_800A501C
cmpwi r3, 0x0
beq @8007B33C
lwz r3, 0x78(r1)
cmplwi r3, 0x0
beq @8007B33C
lwz r4, lbl_8047A648@sda21(r0)
addi r0, r3, 0x5b
clrrwi r0, r0, 5
cmplwi r4, 0x0
stw r0, lbl_8047A650@sda21(r0)
beq @8007B264
lwz r3, lbl_80478980@sda21(r0)
bl fn_8009AAD4
@8007B264
lwz r3, lbl_80478980@sda21(r0)
lwz r4, lbl_8047A650@sda21(r0)
bl fn_8009A9D8
cmplwi r3, 0x0
stw r3, lbl_8047A648@sda21(r0)
beq @8007B33C
lwz r5, lbl_8047A650@sda21(r0)
li r4, 0x0
bl memset
addi r3, r1, 0x44
bl fn_800A50E4
lwz r0, lbl_8047A64C@sda21(r0)
cmpwi r0, 0x0
bne @8007B33C
lis r3, lbl_803FADF8@ha
addi r4, r1, 0x8
addi r3, r3, lbl_803FADF8@l
bl fn_800A501C
cmpwi r3, 0x0
beq @8007B33C
lwz r31, 0x3c(r1)
cmplwi r31, 0x0
beq @8007B33C
addi r0, r31, 0x1f
lwz r4, lbl_8047A648@sda21(r0)
addi r3, r1, 0x8
li r6, 0x0
clrrwi r5, r0, 5
li r7, 0x2
bl fn_800A541C
cmpwi r3, 0x0
blt @8007B33C
cmplw r3, r31
blt @8007B33C
addi r3, r1, 0x8
bl fn_800A50E4
lwz r4, lbl_8047A648@sda21(r0)
mr r5, r31
addi r3, r4, 0x34
bl fn_800C8174
lwz r3, lbl_8047A648@sda21(r0)
li r4, 0x0
li r5, 0x34
bl memset
addi r3, r31, 0x34
lwz r4, lbl_8047A648@sda21(r0)
mr r5, r3
lwz r0, lbl_8047A650@sda21(r0)
add r3, r4, r3
li r4, 0x0
subf r5, r5, r0
bl memset
li r0, 0x1
stw r0, lbl_8047A64C@sda21(r0)
@8007B33C
lwz r0, 0x94(r1)
lwz r31, 0x8c(r1)
mtlr r0
addi r1, r1, 0x90
blr
