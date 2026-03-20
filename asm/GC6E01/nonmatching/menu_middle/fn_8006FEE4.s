stwu r1, -0x30(r1)
mflr r0
stw r0, 0x34(r1)
stmw r24, 0x10(r1)
mr r30, r3
lbz r0, 0x95(r30)
lis r3, lbl_80267EA8@ha
addi r31, r3, lbl_80267EA8@l
extsb r0, r0
cmpwi r0, 0x6
bge @8006FF14
stw r0, lbl_8047A5FC@sda21(r0)
@8006FF14
lwz r3, lbl_8047A5FC@sda21(r0)
bl fn_8006B3C8
mr r28, r3
mr r3, r30
li r4, 0x957
bl fn_801046C8
mr r25, r3
cmplwi r25, 0x0
beq @8006FF70
clrlwi r0, r28, 24
cmplwi r0, 0x0
beq @8006FF58
lwz r0, lbl_8047A5FC@sda21(r0)
addi r3, r31, 0x218
slwi r0, r0, 2
lwzx r3, r3, r0
b @8006FF5C
@8006FF58
li r3, 0x26c
@8006FF5C
bl fn_8005D858
mr r0, r3
mr r3, r25
mr r4, r0
bl fn_80071318
@8006FF70
addi r27, r31, 0x1c8
li r25, 0x0
@8006FF78
lhz r4, 0x0(r27)
mr r3, r30
bl fn_801046C8
mr r29, r3
mr r3, r25
bl fn_8006B3C8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8006FFA4
lwz r0, 0x4(r27)
b @8006FFA8
@8006FFA4
li r0, 0x3daa
@8006FFA8
stw r0, 0x4c(r29)
addi r27, r27, 0x8
addi r25, r25, 0x1
cmpwi r25, 0x6
blt @8006FF78
slwi r0, r25, 3
addi r27, r31, 0x1c8
add r27, r27, r0
b @8006FFE8
@8006FFCC
lhz r4, 0x0(r27)
mr r3, r30
bl fn_801046C8
lwz r0, 0x4(r27)
addi r27, r27, 0x8
addi r25, r25, 0x1
stw r0, 0x4c(r3)
@8006FFE8
cmplwi r25, 0xa
blt @8006FFCC
clrlwi r0, r28, 24
addi r27, r31, 0x230
cntlzw r0, r0
li r25, 0x0
srwi r29, r0, 5
@80070004
lhz r4, 0x0(r27)
mr r3, r30
bl fn_801046C8
clrlwi r4, r29, 24
bl fn_80109220
addi r27, r27, 0x2
addi r25, r25, 0x1
cmplwi r25, 0x5
blt @80070004
lbz r0, 0x1(r30)
extsb r0, r0
cmpwi r0, 0x3
beq @800700AC
bge @8007011C
cmpwi r0, 0x0
beq @80070048
b @8007011C
@80070048
addi r26, r31, 0x23c
li r25, 0x0
addi r29, r31, 0x0
@80070054
srwi r0, r25, 31
mr r27, r26
add r0, r0, r25
li r24, 0x0
srawi r0, r0, 1
slwi r3, r0, 2
addi r28, r3, 0xc
@80070070
lhz r4, 0x0(r27)
mr r3, r30
bl fn_801046C8
lhzx r4, r29, r28
addi r3, r3, 0xc
bl fn_80108518
addi r27, r27, 0x2
addi r24, r24, 0x1
cmplwi r24, 0x5
blt @80070070
addi r26, r26, 0xa
addi r25, r25, 0x1
cmplwi r25, 0x6
blt @80070054
b @8007011C
@800700AC
lbz r0, 0x2(r30)
extsb r0, r0
cmpwi r0, 0x0
bne @8007011C
addi r28, r31, 0x23c
li r25, 0x0
addi r29, r31, 0x0
@800700C8
srwi r0, r25, 31
mr r26, r28
add r0, r0, r25
li r24, 0x0
srawi r0, r0, 1
slwi r3, r0, 2
addi r27, r3, 0xe
@800700E4
lhz r4, 0x0(r26)
mr r3, r30
bl fn_801046C8
lhzx r4, r29, r27
addi r3, r3, 0xc
bl fn_80108518
addi r26, r26, 0x2
addi r24, r24, 0x1
cmplwi r24, 0x5
blt @800700E4
addi r28, r28, 0xa
addi r25, r25, 0x1
cmplwi r25, 0x6
blt @800700C8
@8007011C
mr r3, r30
addi r4, r31, 0x278
li r5, 0xb
bl fn_80070D84
addi r26, r31, 0x2d0
li r27, 0x0
@80070134
cmpwi r27, 0x0
lwz r3, lbl_8047A5FC@sda21(r0)
bne @80070148
li r4, 0x0
b @8007014C
@80070148
li r4, 0x1
@8007014C
bl fn_8006B1F4
lhz r4, 0x0(r26)
mr r24, r3
mr r3, r30
bl fn_801046C8
mr r4, r24
bl fn_80109220
lhz r4, 0x2(r26)
mr r3, r30
bl fn_801046C8
mr r4, r24
bl fn_80109220
lhz r4, 0x4(r26)
mr r3, r30
bl fn_801046C8
clrlwi r0, r24, 24
cmplwi r0, 0x0
beq @8007019C
li r0, -0x1
b @800701A4
@8007019C
lis r4, 0x6060
addi r0, r4, 0x60ff
@800701A4
stw r0, 0x64(r3)
addi r26, r26, 0x6
addi r27, r27, 0x1
cmplwi r27, 0x2
blt @80070134
li r27, 0x0
li r26, lbl_8047C050@sda21
@800701C0
lhz r4, 0x0(r26)
mr r3, r30
bl fn_801046C8
srwi r4, r27, 31
clrlwi r0, r27, 31
xor r0, r0, r4
mr r31, r3
subf r0, r4, r0
cmpwi r0, 0x0
beq @80070210
lfs f1, 0x70(r31)
lfs f0, lbl_8047C078@sda21(r0)
lfd f2, lbl_8047C080@sda21(r0)
fadds f0, f1, f0
stfs f0, 0x70(r31)
lfs f1, 0x70(r31)
bl fn_800CE318
frsp f0, f1
stfs f0, 0x70(r31)
b @80070250
@80070210
lfs f2, 0x70(r31)
lfs f1, lbl_8047C078@sda21(r0)
lfs f0, lbl_8047C088@sda21(r0)
fsubs f1, f2, f1
stfs f1, 0x70(r31)
lfs f1, 0x70(r31)
fcmpo cr0, f0, f1
ble @8007023C
lfs f0, lbl_8047C08C@sda21(r0)
fadds f0, f1, f0
stfs f0, 0x70(r31)
@8007023C
lfs f1, 0x70(r31)
lfd f2, lbl_8047C080@sda21(r0)
bl fn_800CE318
frsp f0, f1
stfs f0, 0x70(r31)
@80070250
addi r26, r26, 0x2
addi r27, r27, 0x1
cmplwi r27, 0x4
blt @800701C0
lmw r24, 0x10(r1)
lwz r0, 0x34(r1)
mtlr r0
addi r1, r1, 0x30
blr
