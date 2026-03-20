stwu r1, -0xa40(r1)
mflr r0
stw r0, 0xa44(r1)
stmw r22, 0xa18(r1)
mr r23, r3
mr r22, r4
mr r25, r5
lis r4, lbl_80268DC0@ha
li r31, 0x1
addi r30, r4, lbl_80268DC0@l
li r4, 0x0
li r5, 0xb20
bl memset
addi r5, r30, 0x0
li r26, 0x0
lwz r24, 0x4(r5)
stw r23, 0x8(r1)
cmpwi r24, 0x10
stw r22, 0xc(r1)
stw r25, 0x10(r1)
stw r26, 0x14(r1)
bge @80080410
lwz r22, 0x8(r5)
b @80080404
@80080370
lwz r7, 0x14(r1)
li r5, 0x0
lwz r6, 0xc(r1)
add r3, r7, r24
subf r0, r7, r3
li r4, lbl_80478948@sda21
mtctr r0
cmpw r7, r3
bge @800803D8
@80080394
srawi r0, r7, 3
clrlwi r3, r7, 29
addze r0, r0
clrlwi r5, r5, 16
lbzx r3, r4, r3
slwi r5, r5, 1
lbzx r0, r6, r0
and r0, r3, r0
cmpwi r0, 0x0
beq @800803C4
li r0, 0x1
b @800803C8
@800803C4
li r0, 0x0
@800803C8
or r0, r5, r0
addi r7, r7, 0x1
clrlwi r5, r0, 16
bdnz @80080394
@800803D8
lwz r0, 0x14(r1)
mr r8, r26
addi r3, r1, 0x8
addi r4, r30, 0x0
add r0, r0, r24
clrlwi r6, r5, 16
stw r0, 0x14(r1)
li r5, -0x1
li r7, 0x0
bl fn_8008102C
addi r26, r26, 0x1
@80080404
cmpw r26, r22
blt @80080370
b @8008053C
@80080410
addi r4, r1, 0x818
li r3, 0x0
b @80080490
@8008041C
mr r9, r3
li r8, 0x0
addi r6, r3, 0x10
li r7, lbl_80478948@sda21
subf r0, r3, r6
mtctr r0
cmpw r3, r6
bge @80080480
@8008043C
srawi r0, r9, 3
clrlwi r6, r9, 29
addze r0, r0
clrlwi r8, r8, 16
lbzx r6, r7, r6
slwi r8, r8, 1
lbzx r0, r22, r0
and r0, r6, r0
cmpwi r0, 0x0
beq @8008046C
li r0, 0x1
b @80080470
@8008046C
li r0, 0x0
@80080470
or r0, r8, r0
addi r9, r9, 0x1
clrlwi r8, r0, 16
bdnz @8008043C
@80080480
sth r8, 0x0(r4)
addi r4, r4, 0x2
addi r3, r3, 0x10
subi r24, r24, 0x10
@80080490
cmpwi r24, 0x10
bgt @8008041C
cmpwi r24, 0x0
beq @80080508
add r6, r3, r24
li r8, 0x0
subf r0, r3, r6
li r7, lbl_80478948@sda21
mtctr r0
cmpw r3, r6
bge @80080500
@800804BC
srawi r0, r3, 3
clrlwi r6, r3, 29
addze r0, r0
clrlwi r8, r8, 16
lbzx r6, r7, r6
slwi r8, r8, 1
lbzx r0, r22, r0
and r0, r6, r0
cmpwi r0, 0x0
beq @800804EC
li r0, 0x1
b @800804F0
@800804EC
li r0, 0x0
@800804F0
or r0, r8, r0
addi r3, r3, 0x1
clrlwi r8, r0, 16
bdnz @800804BC
@80080500
sth r8, 0x0(r4)
addi r4, r4, 0x2
@80080508
lwz r6, 0x14(r1)
li r7, 0x0
lwz r0, 0x4(r5)
addi r3, r1, 0x8
sth r7, 0x0(r4)
addi r4, r30, 0x0
add r0, r6, r0
addi r7, r1, 0x818
stw r0, 0x14(r1)
li r5, -0x1
li r6, 0x0
li r8, -0x1
bl fn_8008102C
@8008053C
li r27, 0x0
stw r27, 0x14(r1)
lwz r0, 0x0(r23)
cmpwi r0, 0x1
beq @80080C4C
bge @80080E8C
cmpwi r0, 0x0
bge @80080560
b @80080E8C
@80080560
addi r28, r30, 0x0
li r22, 0x0
@80080568
lwz r9, 0x4(r28)
li r27, 0x1
cmpwi r9, 0x10
bge @80080640
addi r24, r28, 0x4
addi r25, r28, 0x8
li r26, 0x0
b @80080630
@80080588
lwz r8, 0x14(r1)
li r6, 0x0
lwz r0, 0x0(r24)
lwz r7, 0xc(r1)
add r4, r8, r0
subf r3, r8, r4
li r5, lbl_80478948@sda21
mtctr r3
cmpw r8, r4
bge @800805F4
@800805B0
srawi r3, r8, 3
clrlwi r4, r8, 29
addze r3, r3
clrlwi r6, r6, 16
lbzx r4, r5, r4
slwi r6, r6, 1
lbzx r3, r7, r3
and r3, r4, r3
cmpwi r3, 0x0
beq @800805E0
li r3, 0x1
b @800805E4
@800805E0
li r3, 0x0
@800805E4
or r3, r6, r3
addi r8, r8, 0x1
clrlwi r6, r3, 16
bdnz @800805B0
@800805F4
lwz r5, 0x14(r1)
mr r4, r28
mr r8, r26
addi r3, r1, 0x8
add r0, r5, r0
clrlwi r6, r6, 16
stw r0, 0x14(r1)
li r5, -0x1
li r7, 0x0
bl fn_8008102C
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8008062C
li r27, 0x0
@8008062C
addi r26, r26, 0x1
@80080630
lwz r0, 0x0(r25)
cmpw r26, r0
blt @80080588
b @80080780
@80080640
lwz r4, 0x14(r1)
addi r5, r1, 0x618
lwz r3, 0xc(r1)
b @800806C4
@80080650
mr r10, r4
li r8, 0x0
addi r6, r4, 0x10
li r7, lbl_80478948@sda21
subf r0, r4, r6
mtctr r0
cmpw r4, r6
bge @800806B4
@80080670
srawi r0, r10, 3
clrlwi r6, r10, 29
addze r0, r0
clrlwi r8, r8, 16
lbzx r6, r7, r6
slwi r8, r8, 1
lbzx r0, r3, r0
and r0, r6, r0
cmpwi r0, 0x0
beq @800806A0
li r0, 0x1
b @800806A4
@800806A0
li r0, 0x0
@800806A4
or r0, r8, r0
addi r10, r10, 0x1
clrlwi r8, r0, 16
bdnz @80080670
@800806B4
sth r8, 0x0(r5)
addi r5, r5, 0x2
addi r4, r4, 0x10
subi r9, r9, 0x10
@800806C4
cmpwi r9, 0x10
bgt @80080650
cmpwi r9, 0x0
beq @8008073C
add r6, r4, r9
li r8, 0x0
subf r0, r4, r6
li r7, lbl_80478948@sda21
mtctr r0
cmpw r4, r6
bge @80080734
@800806F0
srawi r0, r4, 3
clrlwi r6, r4, 29
addze r0, r0
clrlwi r8, r8, 16
lbzx r6, r7, r6
slwi r8, r8, 1
lbzx r0, r3, r0
and r0, r6, r0
cmpwi r0, 0x0
beq @80080720
li r0, 0x1
b @80080724
@80080720
li r0, 0x0
@80080724
or r0, r8, r0
addi r4, r4, 0x1
clrlwi r8, r0, 16
bdnz @800806F0
@80080734
sth r8, 0x0(r5)
addi r5, r5, 0x2
@8008073C
lwz r6, 0x14(r1)
li r3, 0x0
lwz r0, 0x4(r28)
mr r4, r28
sth r3, 0x0(r5)
addi r3, r1, 0x8
add r0, r6, r0
addi r7, r1, 0x618
stw r0, 0x14(r1)
li r5, -0x1
li r6, 0x0
li r8, -0x1
bl fn_8008102C
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80080780
li r27, 0x0
@80080780
clrlwi r0, r27, 24
cmplwi r0, 0x0
bne @80080790
li r31, 0x0
@80080790
addi r28, r28, 0xc
addi r22, r22, 0x1
cmplwi r22, 0x28
blt @80080568
li r26, 0x0
@800807A4
mr r28, r26
addi r29, r30, 0x1e0
li r27, 0x0
@800807B0
lwz r9, 0x4(r29)
li r24, 0x1
cmpwi r9, 0x10
bge @80080888
addi r23, r29, 0x4
addi r22, r29, 0x8
li r25, 0x0
b @80080878
@800807D0
lwz r4, 0x14(r1)
li r9, 0x0
lwz r0, 0x0(r23)
lwz r3, 0xc(r1)
add r6, r4, r0
subf r5, r4, r6
li r7, lbl_80478948@sda21
mtctr r5
cmpw r4, r6
bge @8008083C
@800807F8
srawi r5, r4, 3
clrlwi r6, r4, 29
addze r5, r5
clrlwi r8, r9, 16
lbzx r6, r7, r6
slwi r8, r8, 1
lbzx r5, r3, r5
and r5, r6, r5
cmpwi r5, 0x0
beq @80080828
li r5, 0x1
b @8008082C
@80080828
li r5, 0x0
@8008082C
or r5, r8, r5
addi r4, r4, 0x1
clrlwi r9, r5, 16
bdnz @800807F8
@8008083C
lwz r3, 0x14(r1)
mr r4, r29
mr r5, r28
mr r8, r25
add r0, r3, r0
addi r3, r1, 0x8
stw r0, 0x14(r1)
clrlwi r6, r9, 16
li r7, 0x0
bl fn_8008102C
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80080874
li r24, 0x0
@80080874
addi r25, r25, 0x1
@80080878
lwz r0, 0x0(r22)
cmpw r25, r0
blt @800807D0
b @800809C8
@80080888
lwz r4, 0x14(r1)
addi r5, r1, 0x418
lwz r3, 0xc(r1)
b @8008090C
@80080898
mr r10, r4
li r8, 0x0
addi r6, r4, 0x10
li r7, lbl_80478948@sda21
subf r0, r4, r6
mtctr r0
cmpw r4, r6
bge @800808FC
@800808B8
srawi r0, r10, 3
clrlwi r6, r10, 29
addze r0, r0
clrlwi r8, r8, 16
lbzx r6, r7, r6
slwi r8, r8, 1
lbzx r0, r3, r0
and r0, r6, r0
cmpwi r0, 0x0
beq @800808E8
li r0, 0x1
b @800808EC
@800808E8
li r0, 0x0
@800808EC
or r0, r8, r0
addi r10, r10, 0x1
clrlwi r8, r0, 16
bdnz @800808B8
@800808FC
sth r8, 0x0(r5)
addi r5, r5, 0x2
addi r4, r4, 0x10
subi r9, r9, 0x10
@8008090C
cmpwi r9, 0x10
bgt @80080898
cmpwi r9, 0x0
beq @80080984
add r6, r4, r9
li r8, 0x0
subf r0, r4, r6
li r7, lbl_80478948@sda21
mtctr r0
cmpw r4, r6
bge @8008097C
@80080938
srawi r0, r4, 3
clrlwi r6, r4, 29
addze r0, r0
clrlwi r8, r8, 16
lbzx r6, r7, r6
slwi r8, r8, 1
lbzx r0, r3, r0
and r0, r6, r0
cmpwi r0, 0x0
beq @80080968
li r0, 0x1
b @8008096C
@80080968
li r0, 0x0
@8008096C
or r0, r8, r0
addi r4, r4, 0x1
clrlwi r8, r0, 16
bdnz @80080938
@8008097C
sth r8, 0x0(r5)
addi r5, r5, 0x2
@80080984
lwz r3, 0x14(r1)
li r6, 0x0
lwz r0, 0x4(r29)
mr r4, r29
sth r6, 0x0(r5)
mr r5, r28
add r0, r3, r0
addi r3, r1, 0x8
stw r0, 0x14(r1)
addi r7, r1, 0x418
li r6, 0x0
li r8, -0x1
bl fn_8008102C
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @800809C8
li r24, 0x0
@800809C8
clrlwi r0, r24, 24
cmplwi r0, 0x0
bne @800809D8
li r31, 0x0
@800809D8
addi r29, r29, 0xc
addi r27, r27, 0x1
cmplwi r27, 0x8
blt @800807B0
addi r26, r26, 0x1
cmpwi r26, 0x9
blt @800807A4
li r27, 0x0
@800809F8
addi r28, r30, 0x240
li r29, 0x0
@80080A00
lwz r9, 0x4(r28)
li r25, 0x1
cmpwi r9, 0x10
bge @80080ADC
mr r23, r27
addi r22, r28, 0x4
addi r26, r28, 0x8
li r24, 0x0
b @80080ACC
@80080A24
lwz r4, 0x14(r1)
li r9, 0x0
lwz r0, 0x0(r22)
lwz r3, 0xc(r1)
add r6, r4, r0
subf r5, r4, r6
li r7, lbl_80478948@sda21
mtctr r5
cmpw r4, r6
bge @80080A90
@80080A4C
srawi r5, r4, 3
clrlwi r6, r4, 29
addze r5, r5
clrlwi r8, r9, 16
lbzx r6, r7, r6
slwi r8, r8, 1
lbzx r5, r3, r5
and r5, r6, r5
cmpwi r5, 0x0
beq @80080A7C
li r5, 0x1
b @80080A80
@80080A7C
li r5, 0x0
@80080A80
or r5, r8, r5
addi r4, r4, 0x1
clrlwi r9, r5, 16
bdnz @80080A4C
@80080A90
lwz r3, 0x14(r1)
mr r4, r28
mr r5, r23
mr r8, r24
add r0, r3, r0
addi r3, r1, 0x8
stw r0, 0x14(r1)
clrlwi r6, r9, 16
li r7, 0x0
bl fn_8008102C
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80080AC8
li r25, 0x0
@80080AC8
addi r24, r24, 0x1
@80080ACC
lwz r0, 0x0(r26)
cmpw r24, r0
blt @80080A24
b @80080C1C
@80080ADC
lwz r4, 0x14(r1)
addi r5, r1, 0x218
lwz r3, 0xc(r1)
b @80080B60
@80080AEC
mr r10, r4
li r8, 0x0
addi r6, r4, 0x10
li r7, lbl_80478948@sda21
subf r0, r4, r6
mtctr r0
cmpw r4, r6
bge @80080B50
@80080B0C
srawi r0, r10, 3
clrlwi r6, r10, 29
addze r0, r0
clrlwi r8, r8, 16
lbzx r6, r7, r6
slwi r8, r8, 1
lbzx r0, r3, r0
and r0, r6, r0
cmpwi r0, 0x0
beq @80080B3C
li r0, 0x1
b @80080B40
@80080B3C
li r0, 0x0
@80080B40
or r0, r8, r0
addi r10, r10, 0x1
clrlwi r8, r0, 16
bdnz @80080B0C
@80080B50
sth r8, 0x0(r5)
addi r5, r5, 0x2
addi r4, r4, 0x10
subi r9, r9, 0x10
@80080B60
cmpwi r9, 0x10
bgt @80080AEC
cmpwi r9, 0x0
beq @80080BD8
add r6, r4, r9
li r8, 0x0
subf r0, r4, r6
li r7, lbl_80478948@sda21
mtctr r0
cmpw r4, r6
bge @80080BD0
@80080B8C
srawi r0, r4, 3
clrlwi r6, r4, 29
addze r0, r0
clrlwi r8, r8, 16
lbzx r6, r7, r6
slwi r8, r8, 1
lbzx r0, r3, r0
and r0, r6, r0
cmpwi r0, 0x0
beq @80080BBC
li r0, 0x1
b @80080BC0
@80080BBC
li r0, 0x0
@80080BC0
or r0, r8, r0
addi r4, r4, 0x1
clrlwi r8, r0, 16
bdnz @80080B8C
@80080BD0
sth r8, 0x0(r5)
addi r5, r5, 0x2
@80080BD8
lwz r3, 0x14(r1)
li r6, 0x0
lwz r0, 0x4(r28)
mr r4, r28
sth r6, 0x0(r5)
mr r5, r27
add r0, r3, r0
addi r3, r1, 0x8
stw r0, 0x14(r1)
addi r7, r1, 0x218
li r6, 0x0
li r8, -0x1
bl fn_8008102C
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80080C1C
li r25, 0x0
@80080C1C
clrlwi r0, r25, 24
cmplwi r0, 0x0
bne @80080C2C
li r31, 0x0
@80080C2C
addi r28, r28, 0xc
addi r29, r29, 0x1
cmplwi r29, 0x18
blt @80080A00
addi r27, r27, 0x1
cmpwi r27, 0x24
blt @800809F8
b @80080E90
@80080C4C
addi r26, r30, 0x360
@80080C50
lwz r9, 0x4(r26)
li r25, 0x1
cmpwi r9, 0x10
bge @80080D28
addi r22, r26, 0x4
addi r23, r26, 0x8
li r24, 0x0
b @80080D18
@80080C70
lwz r8, 0x14(r1)
li r6, 0x0
lwz r0, 0x0(r22)
lwz r7, 0xc(r1)
add r4, r8, r0
subf r3, r8, r4
li r5, lbl_80478948@sda21
mtctr r3
cmpw r8, r4
bge @80080CDC
@80080C98
srawi r3, r8, 3
clrlwi r4, r8, 29
addze r3, r3
clrlwi r6, r6, 16
lbzx r4, r5, r4
slwi r6, r6, 1
lbzx r3, r7, r3
and r3, r4, r3
cmpwi r3, 0x0
beq @80080CC8
li r3, 0x1
b @80080CCC
@80080CC8
li r3, 0x0
@80080CCC
or r3, r6, r3
addi r8, r8, 0x1
clrlwi r6, r3, 16
bdnz @80080C98
@80080CDC
lwz r5, 0x14(r1)
mr r4, r26
mr r8, r24
addi r3, r1, 0x8
add r0, r5, r0
clrlwi r6, r6, 16
stw r0, 0x14(r1)
li r5, -0x1
li r7, 0x0
bl fn_8008102C
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80080D14
li r25, 0x0
@80080D14
addi r24, r24, 0x1
@80080D18
lwz r0, 0x0(r23)
cmpw r24, r0
blt @80080C70
b @80080E68
@80080D28
lwz r4, 0x14(r1)
addi r5, r1, 0x18
lwz r3, 0xc(r1)
b @80080DAC
@80080D38
mr r10, r4
li r8, 0x0
addi r6, r4, 0x10
li r7, lbl_80478948@sda21
subf r0, r4, r6
mtctr r0
cmpw r4, r6
bge @80080D9C
@80080D58
srawi r0, r10, 3
clrlwi r6, r10, 29
addze r0, r0
clrlwi r8, r8, 16
lbzx r6, r7, r6
slwi r8, r8, 1
lbzx r0, r3, r0
and r0, r6, r0
cmpwi r0, 0x0
beq @80080D88
li r0, 0x1
b @80080D8C
@80080D88
li r0, 0x0
@80080D8C
or r0, r8, r0
addi r10, r10, 0x1
clrlwi r8, r0, 16
bdnz @80080D58
@80080D9C
sth r8, 0x0(r5)
addi r5, r5, 0x2
addi r4, r4, 0x10
subi r9, r9, 0x10
@80080DAC
cmpwi r9, 0x10
bgt @80080D38
cmpwi r9, 0x0
beq @80080E24
add r6, r4, r9
li r8, 0x0
subf r0, r4, r6
li r7, lbl_80478948@sda21
mtctr r0
cmpw r4, r6
bge @80080E1C
@80080DD8
srawi r0, r4, 3
clrlwi r6, r4, 29
addze r0, r0
clrlwi r8, r8, 16
lbzx r6, r7, r6
slwi r8, r8, 1
lbzx r0, r3, r0
and r0, r6, r0
cmpwi r0, 0x0
beq @80080E08
li r0, 0x1
b @80080E0C
@80080E08
li r0, 0x0
@80080E0C
or r0, r8, r0
addi r4, r4, 0x1
clrlwi r8, r0, 16
bdnz @80080DD8
@80080E1C
sth r8, 0x0(r5)
addi r5, r5, 0x2
@80080E24
lwz r6, 0x14(r1)
li r3, 0x0
lwz r0, 0x4(r26)
mr r4, r26
sth r3, 0x0(r5)
addi r3, r1, 0x8
add r0, r6, r0
addi r7, r1, 0x18
stw r0, 0x14(r1)
li r5, -0x1
li r6, 0x0
li r8, -0x1
bl fn_8008102C
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80080E68
li r25, 0x0
@80080E68
clrlwi r0, r25, 24
cmplwi r0, 0x0
bne @80080E78
li r31, 0x0
@80080E78
addi r26, r26, 0xc
addi r27, r27, 0x1
cmplwi r27, 0x3
blt @80080C50
b @80080E90
@80080E8C
li r31, 0x0
@80080E90
clrlwi r0, r31, 24
cmplwi r0, 0x0
bne @80080EA4
li r3, 0x0
b @80080EC4
@80080EA4
lwz r0, 0x10(r1)
lwz r3, 0x14(r1)
slwi r4, r0, 3
subf r0, r3, r4
orc r3, r4, r3
srwi r0, r0, 1
subf r0, r0, r3
srwi r3, r0, 31
@80080EC4
lmw r22, 0xa18(r1)
lwz r0, 0xa44(r1)
mtlr r0
addi r1, r1, 0xa40
blr
