stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stmw r26, 0x8(r1)
mr r26, r3
mr r31, r4
bl fn_8025DA88
cmpwi r3, 0x2
beq @80065A90
bge @80065AA4
cmpwi r3, 0x0
bge @80065A7C
b @80065AA4
@80065A7C
li r30, 0x0
li r29, 0x1
li r28, 0x2
li r27, 0x3
b @80065AB4
@80065A90
li r30, 0x0
li r28, 0x1
li r29, 0x2
li r27, 0x3
b @80065AB4
@80065AA4
li r30, 0x0
li r28, 0x1
li r29, 0x2
li r27, 0x3
@80065AB4
lha r3, 0x6(r31)
subi r0, r3, 0xb3b
cmplwi r0, 0xb5
bgt @800676D8
lis r3, jumptable_802EDB7C@ha
slwi r0, r0, 2
addi r3, r3, jumptable_802EDB7C@l
lwzx r0, r3, r0
mtctr r0
bctr
li r27, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80065B08
bge @80065B08
cmpwi r3, 0x0
bge @80065AFC
b @80065B08
@80065AFC
cmpwi r30, 0x2
blt @80065B08
li r27, 0x0
@80065B08
clrlwi r0, r27, 24
cmplwi r0, 0x0
beq @800676D8
mulli r4, r30, 0x48
lis r3, lbl_803A9F08@ha
addi r0, r3, lbl_803A9F08@l
add r3, r0, r4
addi r3, r3, 0x30
lbz r0, 0x0(r3)
cmplwi r0, 0x0
beq @800676D8
lhz r5, 0x2(r3)
mr r3, r26
mr r4, r31
bl fn_8010B9E8
b @800676D8
li r27, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80065B74
bge @80065B74
cmpwi r3, 0x0
bge @80065B68
b @80065B74
@80065B68
cmpwi r30, 0x2
blt @80065B74
li r27, 0x0
@80065B74
clrlwi r0, r27, 24
cmplwi r0, 0x0
beq @800676D8
mulli r4, r30, 0x48
lis r3, lbl_803A9F08@ha
addi r0, r3, lbl_803A9F08@l
add r3, r0, r4
addi r3, r3, 0x3c
lbz r0, 0x0(r3)
cmplwi r0, 0x0
beq @800676D8
lhz r5, 0x2(r3)
mr r3, r26
mr r4, r31
bl fn_8010B9E8
b @800676D8
li r27, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80065BE0
bge @80065BE0
cmpwi r3, 0x0
bge @80065BD4
b @80065BE0
@80065BD4
cmpwi r30, 0x2
blt @80065BE0
li r27, 0x0
@80065BE0
clrlwi r0, r27, 24
cmplwi r0, 0x0
beq @800676D8
mulli r4, r30, 0x48
lis r3, lbl_803A9F08@ha
addi r0, r3, lbl_803A9F08@l
add r3, r0, r4
addi r3, r3, 0x48
lbz r0, 0x0(r3)
cmplwi r0, 0x0
beq @800676D8
lhz r5, 0x2(r3)
mr r3, r26
mr r4, r31
bl fn_8010B9E8
b @800676D8
li r27, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80065C4C
bge @80065C4C
cmpwi r3, 0x0
bge @80065C40
b @80065C4C
@80065C40
cmpwi r30, 0x2
blt @80065C4C
li r27, 0x0
@80065C4C
clrlwi r0, r27, 24
cmplwi r0, 0x0
beq @800676D8
mulli r4, r30, 0x48
lis r3, lbl_803A9F08@ha
addi r0, r3, lbl_803A9F08@l
add r3, r0, r4
addi r3, r3, 0x54
lbz r0, 0x0(r3)
cmplwi r0, 0x0
beq @800676D8
lhz r5, 0x2(r3)
mr r3, r26
mr r4, r31
bl fn_8010B9E8
b @800676D8
li r27, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80065CB8
bge @80065CB8
cmpwi r3, 0x0
bge @80065CAC
b @80065CB8
@80065CAC
cmpwi r30, 0x2
blt @80065CB8
li r27, 0x0
@80065CB8
clrlwi r0, r27, 24
cmplwi r0, 0x0
beq @800676D8
mulli r4, r30, 0x48
lis r3, lbl_803A9F08@ha
addi r0, r3, lbl_803A9F08@l
add r3, r0, r4
addi r3, r3, 0x60
lbz r0, 0x0(r3)
cmplwi r0, 0x0
beq @800676D8
lhz r5, 0x2(r3)
mr r3, r26
mr r4, r31
bl fn_8010B9E8
b @800676D8
li r27, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80065D24
bge @80065D24
cmpwi r3, 0x0
bge @80065D18
b @80065D24
@80065D18
cmpwi r30, 0x2
blt @80065D24
li r27, 0x0
@80065D24
clrlwi r0, r27, 24
cmplwi r0, 0x0
beq @800676D8
mulli r4, r30, 0x48
lis r3, lbl_803A9F08@ha
addi r0, r3, lbl_803A9F08@l
add r3, r0, r4
addi r3, r3, 0x6c
lbz r0, 0x0(r3)
cmplwi r0, 0x0
beq @800676D8
lhz r5, 0x2(r3)
mr r3, r26
mr r4, r31
bl fn_8010B9E8
b @800676D8
li r27, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80065D90
bge @80065D90
cmpwi r3, 0x0
bge @80065D84
b @80065D90
@80065D84
cmpwi r28, 0x2
blt @80065D90
li r27, 0x0
@80065D90
clrlwi r0, r27, 24
cmplwi r0, 0x0
beq @800676D8
mulli r4, r28, 0x48
lis r3, lbl_803A9F08@ha
addi r0, r3, lbl_803A9F08@l
add r3, r0, r4
addi r3, r3, 0x30
lbz r0, 0x0(r3)
cmplwi r0, 0x0
beq @800676D8
lhz r5, 0x2(r3)
mr r3, r26
mr r4, r31
bl fn_8010B9E8
b @800676D8
li r27, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80065DFC
bge @80065DFC
cmpwi r3, 0x0
bge @80065DF0
b @80065DFC
@80065DF0
cmpwi r28, 0x2
blt @80065DFC
li r27, 0x0
@80065DFC
clrlwi r0, r27, 24
cmplwi r0, 0x0
beq @800676D8
mulli r4, r28, 0x48
lis r3, lbl_803A9F08@ha
addi r0, r3, lbl_803A9F08@l
add r3, r0, r4
addi r3, r3, 0x3c
lbz r0, 0x0(r3)
cmplwi r0, 0x0
beq @800676D8
lhz r5, 0x2(r3)
mr r3, r26
mr r4, r31
bl fn_8010B9E8
b @800676D8
li r27, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80065E68
bge @80065E68
cmpwi r3, 0x0
bge @80065E5C
b @80065E68
@80065E5C
cmpwi r28, 0x2
blt @80065E68
li r27, 0x0
@80065E68
clrlwi r0, r27, 24
cmplwi r0, 0x0
beq @800676D8
mulli r4, r28, 0x48
lis r3, lbl_803A9F08@ha
addi r0, r3, lbl_803A9F08@l
add r3, r0, r4
addi r3, r3, 0x48
lbz r0, 0x0(r3)
cmplwi r0, 0x0
beq @800676D8
lhz r5, 0x2(r3)
mr r3, r26
mr r4, r31
bl fn_8010B9E8
b @800676D8
li r27, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80065ED4
bge @80065ED4
cmpwi r3, 0x0
bge @80065EC8
b @80065ED4
@80065EC8
cmpwi r28, 0x2
blt @80065ED4
li r27, 0x0
@80065ED4
clrlwi r0, r27, 24
cmplwi r0, 0x0
beq @800676D8
mulli r4, r28, 0x48
lis r3, lbl_803A9F08@ha
addi r0, r3, lbl_803A9F08@l
add r3, r0, r4
addi r3, r3, 0x54
lbz r0, 0x0(r3)
cmplwi r0, 0x0
beq @800676D8
lhz r5, 0x2(r3)
mr r3, r26
mr r4, r31
bl fn_8010B9E8
b @800676D8
li r27, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80065F40
bge @80065F40
cmpwi r3, 0x0
bge @80065F34
b @80065F40
@80065F34
cmpwi r28, 0x2
blt @80065F40
li r27, 0x0
@80065F40
clrlwi r0, r27, 24
cmplwi r0, 0x0
beq @800676D8
mulli r4, r28, 0x48
lis r3, lbl_803A9F08@ha
addi r0, r3, lbl_803A9F08@l
add r3, r0, r4
addi r3, r3, 0x60
lbz r0, 0x0(r3)
cmplwi r0, 0x0
beq @800676D8
lhz r5, 0x2(r3)
mr r3, r26
mr r4, r31
bl fn_8010B9E8
b @800676D8
li r27, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80065FAC
bge @80065FAC
cmpwi r3, 0x0
bge @80065FA0
b @80065FAC
@80065FA0
cmpwi r28, 0x2
blt @80065FAC
li r27, 0x0
@80065FAC
clrlwi r0, r27, 24
cmplwi r0, 0x0
beq @800676D8
mulli r4, r28, 0x48
lis r3, lbl_803A9F08@ha
addi r0, r3, lbl_803A9F08@l
add r3, r0, r4
addi r3, r3, 0x6c
lbz r0, 0x0(r3)
cmplwi r0, 0x0
beq @800676D8
lhz r5, 0x2(r3)
mr r3, r26
mr r4, r31
bl fn_8010B9E8
b @800676D8
li r27, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80066018
bge @80066018
cmpwi r3, 0x0
bge @8006600C
b @80066018
@8006600C
cmpwi r29, 0x2
blt @80066018
li r27, 0x0
@80066018
clrlwi r0, r27, 24
cmplwi r0, 0x0
beq @800676D8
mulli r4, r29, 0x48
lis r3, lbl_803A9F08@ha
addi r0, r3, lbl_803A9F08@l
add r3, r0, r4
addi r3, r3, 0x30
lbz r0, 0x0(r3)
cmplwi r0, 0x0
beq @800676D8
lhz r5, 0x2(r3)
mr r3, r26
mr r4, r31
bl fn_8010B9E8
b @800676D8
li r27, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80066084
bge @80066084
cmpwi r3, 0x0
bge @80066078
b @80066084
@80066078
cmpwi r29, 0x2
blt @80066084
li r27, 0x0
@80066084
clrlwi r0, r27, 24
cmplwi r0, 0x0
beq @800676D8
mulli r4, r29, 0x48
lis r3, lbl_803A9F08@ha
addi r0, r3, lbl_803A9F08@l
add r3, r0, r4
addi r3, r3, 0x3c
lbz r0, 0x0(r3)
cmplwi r0, 0x0
beq @800676D8
lhz r5, 0x2(r3)
mr r3, r26
mr r4, r31
bl fn_8010B9E8
b @800676D8
li r27, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @800660F0
bge @800660F0
cmpwi r3, 0x0
bge @800660E4
b @800660F0
@800660E4
cmpwi r29, 0x2
blt @800660F0
li r27, 0x0
@800660F0
clrlwi r0, r27, 24
cmplwi r0, 0x0
beq @800676D8
mulli r4, r29, 0x48
lis r3, lbl_803A9F08@ha
addi r0, r3, lbl_803A9F08@l
add r3, r0, r4
addi r3, r3, 0x48
lbz r0, 0x0(r3)
cmplwi r0, 0x0
beq @800676D8
lhz r5, 0x2(r3)
mr r3, r26
mr r4, r31
bl fn_8010B9E8
b @800676D8
li r27, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @8006615C
bge @8006615C
cmpwi r3, 0x0
bge @80066150
b @8006615C
@80066150
cmpwi r29, 0x2
blt @8006615C
li r27, 0x0
@8006615C
clrlwi r0, r27, 24
cmplwi r0, 0x0
beq @800676D8
mulli r4, r29, 0x48
lis r3, lbl_803A9F08@ha
addi r0, r3, lbl_803A9F08@l
add r3, r0, r4
addi r3, r3, 0x54
lbz r0, 0x0(r3)
cmplwi r0, 0x0
beq @800676D8
lhz r5, 0x2(r3)
mr r3, r26
mr r4, r31
bl fn_8010B9E8
b @800676D8
li r27, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @800661C8
bge @800661C8
cmpwi r3, 0x0
bge @800661BC
b @800661C8
@800661BC
cmpwi r29, 0x2
blt @800661C8
li r27, 0x0
@800661C8
clrlwi r0, r27, 24
cmplwi r0, 0x0
beq @800676D8
mulli r4, r29, 0x48
lis r3, lbl_803A9F08@ha
addi r0, r3, lbl_803A9F08@l
add r3, r0, r4
addi r3, r3, 0x60
lbz r0, 0x0(r3)
cmplwi r0, 0x0
beq @800676D8
lhz r5, 0x2(r3)
mr r3, r26
mr r4, r31
bl fn_8010B9E8
b @800676D8
li r27, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80066234
bge @80066234
cmpwi r3, 0x0
bge @80066228
b @80066234
@80066228
cmpwi r29, 0x2
blt @80066234
li r27, 0x0
@80066234
clrlwi r0, r27, 24
cmplwi r0, 0x0
beq @800676D8
mulli r4, r29, 0x48
lis r3, lbl_803A9F08@ha
addi r0, r3, lbl_803A9F08@l
add r3, r0, r4
addi r3, r3, 0x6c
lbz r0, 0x0(r3)
cmplwi r0, 0x0
beq @800676D8
lhz r5, 0x2(r3)
mr r3, r26
mr r4, r31
bl fn_8010B9E8
b @800676D8
li r28, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @800662A0
bge @800662A0
cmpwi r3, 0x0
bge @80066294
b @800662A0
@80066294
cmpwi r27, 0x2
blt @800662A0
li r28, 0x0
@800662A0
clrlwi r0, r28, 24
cmplwi r0, 0x0
beq @800676D8
mulli r4, r27, 0x48
lis r3, lbl_803A9F08@ha
addi r0, r3, lbl_803A9F08@l
add r3, r0, r4
addi r3, r3, 0x30
lbz r0, 0x0(r3)
cmplwi r0, 0x0
beq @800676D8
lhz r5, 0x2(r3)
mr r3, r26
mr r4, r31
bl fn_8010B9E8
b @800676D8
li r28, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @8006630C
bge @8006630C
cmpwi r3, 0x0
bge @80066300
b @8006630C
@80066300
cmpwi r27, 0x2
blt @8006630C
li r28, 0x0
@8006630C
clrlwi r0, r28, 24
cmplwi r0, 0x0
beq @800676D8
mulli r4, r27, 0x48
lis r3, lbl_803A9F08@ha
addi r0, r3, lbl_803A9F08@l
add r3, r0, r4
addi r3, r3, 0x3c
lbz r0, 0x0(r3)
cmplwi r0, 0x0
beq @800676D8
lhz r5, 0x2(r3)
mr r3, r26
mr r4, r31
bl fn_8010B9E8
b @800676D8
li r28, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80066378
bge @80066378
cmpwi r3, 0x0
bge @8006636C
b @80066378
@8006636C
cmpwi r27, 0x2
blt @80066378
li r28, 0x0
@80066378
clrlwi r0, r28, 24
cmplwi r0, 0x0
beq @800676D8
mulli r4, r27, 0x48
lis r3, lbl_803A9F08@ha
addi r0, r3, lbl_803A9F08@l
add r3, r0, r4
addi r3, r3, 0x48
lbz r0, 0x0(r3)
cmplwi r0, 0x0
beq @800676D8
lhz r5, 0x2(r3)
mr r3, r26
mr r4, r31
bl fn_8010B9E8
b @800676D8
li r28, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @800663E4
bge @800663E4
cmpwi r3, 0x0
bge @800663D8
b @800663E4
@800663D8
cmpwi r27, 0x2
blt @800663E4
li r28, 0x0
@800663E4
clrlwi r0, r28, 24
cmplwi r0, 0x0
beq @800676D8
mulli r4, r27, 0x48
lis r3, lbl_803A9F08@ha
addi r0, r3, lbl_803A9F08@l
add r3, r0, r4
addi r3, r3, 0x54
lbz r0, 0x0(r3)
cmplwi r0, 0x0
beq @800676D8
lhz r5, 0x2(r3)
mr r3, r26
mr r4, r31
bl fn_8010B9E8
b @800676D8
li r28, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80066450
bge @80066450
cmpwi r3, 0x0
bge @80066444
b @80066450
@80066444
cmpwi r27, 0x2
blt @80066450
li r28, 0x0
@80066450
clrlwi r0, r28, 24
cmplwi r0, 0x0
beq @800676D8
mulli r4, r27, 0x48
lis r3, lbl_803A9F08@ha
addi r0, r3, lbl_803A9F08@l
add r3, r0, r4
addi r3, r3, 0x60
lbz r0, 0x0(r3)
cmplwi r0, 0x0
beq @800676D8
lhz r5, 0x2(r3)
mr r3, r26
mr r4, r31
bl fn_8010B9E8
b @800676D8
li r28, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @800664BC
bge @800664BC
cmpwi r3, 0x0
bge @800664B0
b @800664BC
@800664B0
cmpwi r27, 0x2
blt @800664BC
li r28, 0x0
@800664BC
clrlwi r0, r28, 24
cmplwi r0, 0x0
beq @800676D8
mulli r4, r27, 0x48
lis r3, lbl_803A9F08@ha
addi r0, r3, lbl_803A9F08@l
add r3, r0, r4
addi r3, r3, 0x6c
lbz r0, 0x0(r3)
cmplwi r0, 0x0
beq @800676D8
lhz r5, 0x2(r3)
mr r3, r26
mr r4, r31
bl fn_8010B9E8
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r30
bl fn_80068DBC
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r30
li r6, 0x0
bl fn_80068BB0
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r30
bl fn_800689FC
b @800676D8
li r26, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80066568
bge @80066568
cmpwi r3, 0x0
bge @8006655C
b @80066568
@8006655C
cmpwi r30, 0x2
blt @80066568
li r26, 0x0
@80066568
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @800676D8
bl fn_8006B1D4
clrlwi r26, r3, 16
mr r3, r30
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r26
bge @80066594
b @80066598
@80066594
mr r0, r26
@80066598
clrlwi r0, r0, 16
cmpwi r0, 0x0
ble @800665B8
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@800665B8
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
li r26, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @800665F8
bge @800665F8
cmpwi r3, 0x0
bge @800665EC
b @800665F8
@800665EC
cmpwi r30, 0x2
blt @800665F8
li r26, 0x0
@800665F8
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @800676D8
bl fn_8006B1D4
clrlwi r26, r3, 16
mr r3, r30
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r26
bge @80066624
b @80066628
@80066624
mr r0, r26
@80066628
clrlwi r0, r0, 16
cmpwi r0, 0x1
ble @80066648
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@80066648
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
li r26, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80066688
bge @80066688
cmpwi r3, 0x0
bge @8006667C
b @80066688
@8006667C
cmpwi r30, 0x2
blt @80066688
li r26, 0x0
@80066688
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @800676D8
bl fn_8006B1D4
clrlwi r26, r3, 16
mr r3, r30
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r26
bge @800666B4
b @800666B8
@800666B4
mr r0, r26
@800666B8
clrlwi r0, r0, 16
cmpwi r0, 0x2
ble @800666D8
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@800666D8
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
li r26, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80066718
bge @80066718
cmpwi r3, 0x0
bge @8006670C
b @80066718
@8006670C
cmpwi r30, 0x2
blt @80066718
li r26, 0x0
@80066718
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @800676D8
bl fn_8006B1D4
clrlwi r26, r3, 16
mr r3, r30
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r26
bge @80066744
b @80066748
@80066744
mr r0, r26
@80066748
clrlwi r0, r0, 16
cmpwi r0, 0x3
ble @80066768
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@80066768
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
li r26, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @800667A8
bge @800667A8
cmpwi r3, 0x0
bge @8006679C
b @800667A8
@8006679C
cmpwi r30, 0x2
blt @800667A8
li r26, 0x0
@800667A8
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @800676D8
bl fn_8006B1D4
clrlwi r26, r3, 16
mr r3, r30
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r26
bge @800667D4
b @800667D8
@800667D4
mr r0, r26
@800667D8
clrlwi r0, r0, 16
cmpwi r0, 0x4
ble @800667F8
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@800667F8
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
li r26, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80066838
bge @80066838
cmpwi r3, 0x0
bge @8006682C
b @80066838
@8006682C
cmpwi r30, 0x2
blt @80066838
li r26, 0x0
@80066838
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @800676D8
bl fn_8006B1D4
clrlwi r26, r3, 16
mr r3, r30
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r26
bge @80066864
b @80066868
@80066864
mr r0, r26
@80066868
clrlwi r0, r0, 16
cmpwi r0, 0x5
ble @80066888
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@80066888
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r30
li r6, 0x0
bl fn_80068794
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r30
li r6, 0x1
bl fn_80068794
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r30
li r6, 0x2
bl fn_80068794
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r30
li r6, 0x3
bl fn_80068794
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r30
li r6, 0x4
bl fn_80068794
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r30
li r6, 0x5
bl fn_80068794
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r28
bl fn_80068DBC
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r28
li r6, 0x0
bl fn_80068BB0
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r28
bl fn_800689FC
b @800676D8
li r26, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80066998
bge @80066998
cmpwi r3, 0x0
bge @8006698C
b @80066998
@8006698C
cmpwi r28, 0x2
blt @80066998
li r26, 0x0
@80066998
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @800676D8
bl fn_8006B1D4
clrlwi r26, r3, 16
mr r3, r28
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r26
bge @800669C4
b @800669C8
@800669C4
mr r0, r26
@800669C8
clrlwi r0, r0, 16
cmpwi r0, 0x0
ble @800669E8
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@800669E8
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
li r26, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80066A28
bge @80066A28
cmpwi r3, 0x0
bge @80066A1C
b @80066A28
@80066A1C
cmpwi r28, 0x2
blt @80066A28
li r26, 0x0
@80066A28
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @800676D8
bl fn_8006B1D4
clrlwi r26, r3, 16
mr r3, r28
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r26
bge @80066A54
b @80066A58
@80066A54
mr r0, r26
@80066A58
clrlwi r0, r0, 16
cmpwi r0, 0x1
ble @80066A78
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@80066A78
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
li r26, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80066AB8
bge @80066AB8
cmpwi r3, 0x0
bge @80066AAC
b @80066AB8
@80066AAC
cmpwi r28, 0x2
blt @80066AB8
li r26, 0x0
@80066AB8
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @800676D8
bl fn_8006B1D4
clrlwi r26, r3, 16
mr r3, r28
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r26
bge @80066AE4
b @80066AE8
@80066AE4
mr r0, r26
@80066AE8
clrlwi r0, r0, 16
cmpwi r0, 0x2
ble @80066B08
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@80066B08
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
li r26, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80066B48
bge @80066B48
cmpwi r3, 0x0
bge @80066B3C
b @80066B48
@80066B3C
cmpwi r28, 0x2
blt @80066B48
li r26, 0x0
@80066B48
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @800676D8
bl fn_8006B1D4
clrlwi r26, r3, 16
mr r3, r28
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r26
bge @80066B74
b @80066B78
@80066B74
mr r0, r26
@80066B78
clrlwi r0, r0, 16
cmpwi r0, 0x3
ble @80066B98
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@80066B98
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
li r26, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80066BD8
bge @80066BD8
cmpwi r3, 0x0
bge @80066BCC
b @80066BD8
@80066BCC
cmpwi r28, 0x2
blt @80066BD8
li r26, 0x0
@80066BD8
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @800676D8
bl fn_8006B1D4
clrlwi r26, r3, 16
mr r3, r28
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r26
bge @80066C04
b @80066C08
@80066C04
mr r0, r26
@80066C08
clrlwi r0, r0, 16
cmpwi r0, 0x4
ble @80066C28
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@80066C28
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
li r26, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80066C68
bge @80066C68
cmpwi r3, 0x0
bge @80066C5C
b @80066C68
@80066C5C
cmpwi r28, 0x2
blt @80066C68
li r26, 0x0
@80066C68
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @800676D8
bl fn_8006B1D4
clrlwi r26, r3, 16
mr r3, r28
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r26
bge @80066C94
b @80066C98
@80066C94
mr r0, r26
@80066C98
clrlwi r0, r0, 16
cmpwi r0, 0x5
ble @80066CB8
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@80066CB8
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r28
li r6, 0x0
bl fn_80068794
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r28
li r6, 0x1
bl fn_80068794
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r28
li r6, 0x2
bl fn_80068794
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r28
li r6, 0x3
bl fn_80068794
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r28
li r6, 0x4
bl fn_80068794
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r28
li r6, 0x5
bl fn_80068794
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r29
bl fn_80068DBC
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r29
li r6, 0x0
bl fn_80068BB0
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r29
bl fn_800689FC
b @800676D8
li r26, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80066DC8
bge @80066DC8
cmpwi r3, 0x0
bge @80066DBC
b @80066DC8
@80066DBC
cmpwi r29, 0x2
blt @80066DC8
li r26, 0x0
@80066DC8
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @800676D8
bl fn_8006B1D4
clrlwi r26, r3, 16
mr r3, r29
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r26
bge @80066DF4
b @80066DF8
@80066DF4
mr r0, r26
@80066DF8
clrlwi r0, r0, 16
cmpwi r0, 0x0
ble @80066E18
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@80066E18
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
li r26, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80066E58
bge @80066E58
cmpwi r3, 0x0
bge @80066E4C
b @80066E58
@80066E4C
cmpwi r29, 0x2
blt @80066E58
li r26, 0x0
@80066E58
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @800676D8
bl fn_8006B1D4
clrlwi r26, r3, 16
mr r3, r29
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r26
bge @80066E84
b @80066E88
@80066E84
mr r0, r26
@80066E88
clrlwi r0, r0, 16
cmpwi r0, 0x1
ble @80066EA8
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@80066EA8
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
li r26, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80066EE8
bge @80066EE8
cmpwi r3, 0x0
bge @80066EDC
b @80066EE8
@80066EDC
cmpwi r29, 0x2
blt @80066EE8
li r26, 0x0
@80066EE8
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @800676D8
bl fn_8006B1D4
clrlwi r26, r3, 16
mr r3, r29
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r26
bge @80066F14
b @80066F18
@80066F14
mr r0, r26
@80066F18
clrlwi r0, r0, 16
cmpwi r0, 0x2
ble @80066F38
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@80066F38
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
li r26, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80066F78
bge @80066F78
cmpwi r3, 0x0
bge @80066F6C
b @80066F78
@80066F6C
cmpwi r29, 0x2
blt @80066F78
li r26, 0x0
@80066F78
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @800676D8
bl fn_8006B1D4
clrlwi r26, r3, 16
mr r3, r29
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r26
bge @80066FA4
b @80066FA8
@80066FA4
mr r0, r26
@80066FA8
clrlwi r0, r0, 16
cmpwi r0, 0x3
ble @80066FC8
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@80066FC8
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
li r26, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80067008
bge @80067008
cmpwi r3, 0x0
bge @80066FFC
b @80067008
@80066FFC
cmpwi r29, 0x2
blt @80067008
li r26, 0x0
@80067008
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @800676D8
bl fn_8006B1D4
clrlwi r26, r3, 16
mr r3, r29
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r26
bge @80067034
b @80067038
@80067034
mr r0, r26
@80067038
clrlwi r0, r0, 16
cmpwi r0, 0x4
ble @80067058
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@80067058
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
li r26, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80067098
bge @80067098
cmpwi r3, 0x0
bge @8006708C
b @80067098
@8006708C
cmpwi r29, 0x2
blt @80067098
li r26, 0x0
@80067098
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @800676D8
bl fn_8006B1D4
clrlwi r26, r3, 16
mr r3, r29
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r26
bge @800670C4
b @800670C8
@800670C4
mr r0, r26
@800670C8
clrlwi r0, r0, 16
cmpwi r0, 0x5
ble @800670E8
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@800670E8
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r29
li r6, 0x0
bl fn_80068794
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r29
li r6, 0x1
bl fn_80068794
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r29
li r6, 0x2
bl fn_80068794
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r29
li r6, 0x3
bl fn_80068794
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r29
li r6, 0x4
bl fn_80068794
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r29
li r6, 0x5
bl fn_80068794
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r27
bl fn_80068DBC
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r27
li r6, 0x0
bl fn_80068BB0
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r27
bl fn_800689FC
b @800676D8
li r26, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @800671F8
bge @800671F8
cmpwi r3, 0x0
bge @800671EC
b @800671F8
@800671EC
cmpwi r27, 0x2
blt @800671F8
li r26, 0x0
@800671F8
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @800676D8
bl fn_8006B1D4
clrlwi r26, r3, 16
mr r3, r27
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r26
bge @80067224
b @80067228
@80067224
mr r0, r26
@80067228
clrlwi r0, r0, 16
cmpwi r0, 0x0
ble @80067248
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@80067248
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
li r26, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80067288
bge @80067288
cmpwi r3, 0x0
bge @8006727C
b @80067288
@8006727C
cmpwi r27, 0x2
blt @80067288
li r26, 0x0
@80067288
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @800676D8
bl fn_8006B1D4
clrlwi r26, r3, 16
mr r3, r27
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r26
bge @800672B4
b @800672B8
@800672B4
mr r0, r26
@800672B8
clrlwi r0, r0, 16
cmpwi r0, 0x1
ble @800672D8
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@800672D8
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
li r26, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80067318
bge @80067318
cmpwi r3, 0x0
bge @8006730C
b @80067318
@8006730C
cmpwi r27, 0x2
blt @80067318
li r26, 0x0
@80067318
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @800676D8
bl fn_8006B1D4
clrlwi r26, r3, 16
mr r3, r27
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r26
bge @80067344
b @80067348
@80067344
mr r0, r26
@80067348
clrlwi r0, r0, 16
cmpwi r0, 0x2
ble @80067368
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@80067368
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
li r26, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @800673A8
bge @800673A8
cmpwi r3, 0x0
bge @8006739C
b @800673A8
@8006739C
cmpwi r27, 0x2
blt @800673A8
li r26, 0x0
@800673A8
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @800676D8
bl fn_8006B1D4
clrlwi r26, r3, 16
mr r3, r27
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r26
bge @800673D4
b @800673D8
@800673D4
mr r0, r26
@800673D8
clrlwi r0, r0, 16
cmpwi r0, 0x3
ble @800673F8
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@800673F8
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
li r26, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @80067438
bge @80067438
cmpwi r3, 0x0
bge @8006742C
b @80067438
@8006742C
cmpwi r27, 0x2
blt @80067438
li r26, 0x0
@80067438
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @800676D8
bl fn_8006B1D4
clrlwi r26, r3, 16
mr r3, r27
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r26
bge @80067464
b @80067468
@80067464
mr r0, r26
@80067468
clrlwi r0, r0, 16
cmpwi r0, 0x4
ble @80067488
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@80067488
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
li r26, 0x1
bl fn_8025DA88
cmpwi r3, 0x2
beq @800674C8
bge @800674C8
cmpwi r3, 0x0
bge @800674BC
b @800674C8
@800674BC
cmpwi r27, 0x2
blt @800674C8
li r26, 0x0
@800674C8
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @800676D8
bl fn_8006B1D4
clrlwi r26, r3, 16
mr r3, r27
bl fn_8025D89C
clrlwi r0, r3, 16
cmplw r0, r26
bge @800674F4
b @800674F8
@800674F4
mr r0, r26
@800674F8
clrlwi r0, r0, 16
cmpwi r0, 0x5
ble @80067518
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@80067518
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r27
li r6, 0x0
bl fn_80068794
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r27
li r6, 0x1
bl fn_80068794
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r27
li r6, 0x2
bl fn_80068794
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r27
li r6, 0x3
bl fn_80068794
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r27
li r6, 0x4
bl fn_80068794
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r27
li r6, 0x5
bl fn_80068794
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r30
li r6, 0x0
bl fn_800688C4
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r28
li r6, 0x1
bl fn_800688C4
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r29
li r6, 0x2
bl fn_800688C4
b @800676D8
mr r3, r26
mr r4, r31
mr r5, r27
li r6, 0x3
bl fn_800688C4
b @800676D8
li r3, 0x0
bl fn_8025D9F0
clrlwi r0, r3, 16
cmpwi r0, 0x0
beq @80067634
b @80067648
@80067634
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@80067648
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
li r3, 0x0
bl fn_8025D9F0
clrlwi r0, r3, 16
cmpwi r0, 0x0
beq @80067674
b @80067688
@80067674
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@80067688
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
li r3, 0x0
bl fn_8025D9F0
clrlwi r0, r3, 16
cmpwi r0, 0x0
beq @800676B4
b @800676C8
@800676B4
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @800676D8
@800676C8
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
@800676D8
lmw r26, 0x8(r1)
lwz r0, 0x24(r1)
mtlr r0
addi r1, r1, 0x20
blr
