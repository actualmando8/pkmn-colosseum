stwu r1, -0x60(r1)
mflr r0
stw r0, 0x64(r1)
stmw r16, 0x20(r1)
mr r31, r3
cmpwi r4, 0x0
lis r3, lbl_80268780@ha
addi r30, r3, lbl_80268780@l
beq @80074844
lis r3, lbl_803B6E40@ha
lwz r18, lbl_8047A604@sda21(r0)
addi r0, r3, lbl_803B6E40@l
mr r22, r0
b @80074854
@80074844
lis r3, lbl_803D6E40@ha
lwz r18, lbl_8047A608@sda21(r0)
addi r0, r3, lbl_803D6E40@l
mr r22, r0
@80074854
li r16, 0x0
@80074858
cmplwi r16, 0x20
ble @8007486C
lis r3, 0xdd65
addi r20, r3, 0x4321
b @80074878
@8007486C
bl OSGetTick
clrlwi r0, r3, 8
oris r20, r0, 0xdd00
@80074878
li r3, 0x0
mr r5, r20
mr r4, r3
li r0, 0x4
mtctr r0
@8007488C
clrlwi r0, r5, 31
cmplwi r0, 0x0
beq @8007489C
addi r3, r3, 0x1
@8007489C
srwi r5, r5, 1
clrlwi r0, r5, 31
cmplwi r0, 0x0
beq @800748B0
addi r3, r3, 0x1
@800748B0
srwi r5, r5, 1
clrlwi r0, r5, 31
cmplwi r0, 0x0
beq @800748C4
addi r3, r3, 0x1
@800748C4
srwi r5, r5, 1
clrlwi r0, r5, 31
cmplwi r0, 0x0
beq @800748D8
addi r3, r3, 0x1
@800748D8
srwi r5, r5, 1
clrlwi r0, r5, 31
cmplwi r0, 0x0
beq @800748EC
addi r3, r3, 0x1
@800748EC
srwi r5, r5, 1
clrlwi r0, r5, 31
cmplwi r0, 0x0
beq @80074900
addi r3, r3, 0x1
@80074900
srwi r5, r5, 1
clrlwi r0, r5, 31
cmplwi r0, 0x0
beq @80074914
addi r3, r3, 0x1
@80074914
srwi r5, r5, 1
clrlwi r0, r5, 31
cmplwi r0, 0x0
beq @80074928
addi r3, r3, 0x1
@80074928
srwi r5, r5, 1
addi r4, r4, 0x7
bdnz @8007488C
cmplwi r3, 0xa
addi r16, r16, 0x1
blt @80074858
cmplwi r3, 0x18
bgt @80074858
srwi r3, r20, 24
rlwinm r0, r20, 24, 16, 23
rlwinm r4, r20, 8, 8, 15
slwi r5, r20, 24
or r0, r3, r0
mr r3, r31
or r0, r4, r0
addi r4, r1, 0xc
or r0, r5, r0
addi r5, r1, 0x8
stw r0, 0xc(r1)
bl fn_8025F648
cmpwi r3, 0x0
beq @80074988
li r3, 0x1
b @8007537C
@80074988
lis r4, 0x8000
lis r3, 0x1062
lwz r0, 0xf8(r4)
addi r3, r3, 0x4dd3
srwi r0, r0, 2
mulhwu r0, r3, r0
srwi r0, r0, 6
mulli r16, r0, 0x64
bl OSGetTick
lis r5, lbl_803B6E18@ha
lis r4, lbl_803B6E08@ha
slwi r6, r31, 3
mr r17, r3
addi r0, r5, lbl_803B6E18@l
slwi r21, r31, 2
add r29, r0, r6
addi r19, r4, lbl_803B6E08@l
addi r28, r29, 0x4
@800749D0
bl OSGetTick
subf r0, r17, r3
cmplw r0, r16
ble @800749E8
li r3, 0x2
b @8007537C
@800749E8
mr r3, r31
addi r4, r1, 0x8
bl fn_8025F3F4
cmpwi r3, 0x0
beq @80074A04
li r3, 0x3
b @8007537C
@80074A04
lbz r0, 0x8(r1)
cmplwi r0, 0x38
beq @80074A40
lwz r12, 0x0(r29)
cmplwi r12, 0x0
beq @80074A2C
mr r3, r31
lwz r4, 0x0(r28)
mtctr r12
bctrl
@80074A2C
lwzx r0, r19, r21
cmpwi r0, 0x0
beq @800749D0
li r3, 0x3e8
b @8007537C
@80074A40
mr r3, r31
addi r4, r1, 0x10
addi r5, r1, 0x8
bl fn_8025F584
cmpwi r3, 0x0
beq @80074A60
li r3, 0x4
b @8007537C
@80074A60
lwz r5, 0x10(r1)
srwi r3, r5, 24
rlwinm r0, r5, 24, 16, 23
rlwinm r4, r5, 8, 8, 15
slwi r5, r5, 24
or r0, r3, r0
or r0, r4, r0
or r4, r5, r0
clrlwi r0, r4, 24
cmplwi r0, 0xee
beq @80074AA0
addi r3, r30, 0x68
crclr 6
bl OSReport
li r21, 0x0
b @80074B80
@80074AA0
li r3, 0x0
clrrwi r21, r4, 8
mr r5, r3
li r0, 0x3
mtctr r0
@80074AB4
clrrwi r0, r4, 31
cmplwi r0, 0x0
beq @80074AC4
addi r3, r3, 0x1
@80074AC4
slwi r4, r4, 1
clrrwi r0, r4, 31
cmplwi r0, 0x0
beq @80074AD8
addi r3, r3, 0x1
@80074AD8
slwi r4, r4, 1
clrrwi r0, r4, 31
cmplwi r0, 0x0
beq @80074AEC
addi r3, r3, 0x1
@80074AEC
slwi r4, r4, 1
clrrwi r0, r4, 31
cmplwi r0, 0x0
beq @80074B00
addi r3, r3, 0x1
@80074B00
slwi r4, r4, 1
clrrwi r0, r4, 31
cmplwi r0, 0x0
beq @80074B14
addi r3, r3, 0x1
@80074B14
slwi r4, r4, 1
clrrwi r0, r4, 31
cmplwi r0, 0x0
beq @80074B28
addi r3, r3, 0x1
@80074B28
slwi r4, r4, 1
clrrwi r0, r4, 31
cmplwi r0, 0x0
beq @80074B3C
addi r3, r3, 0x1
@80074B3C
slwi r4, r4, 1
clrrwi r0, r4, 31
cmplwi r0, 0x0
beq @80074B50
addi r3, r3, 0x1
@80074B50
slwi r4, r4, 1
addi r5, r5, 0x7
bdnz @80074AB4
cmplwi r3, 0x7
blt @80074B6C
cmplwi r3, 0xe
ble @80074B80
@80074B6C
mr r4, r21
addi r3, r30, 0x98
crclr 6
bl OSReport
li r21, 0x0
@80074B80
cmplwi r21, 0x0
bne @80074B90
li r3, 0x5
b @8007537C
@80074B90
addi r0, r18, 0x7
mr r3, r31
clrrwi r27, r0, 3
addi r4, r1, 0xc
srwi r6, r27, 3
addi r5, r1, 0x8
subi r8, r6, 0x1
srwi r6, r8, 24
rlwinm r0, r8, 24, 16, 23
rlwinm r7, r8, 8, 8, 15
or r0, r6, r0
slwi r6, r8, 24
or r0, r7, r0
or r0, r6, r0
stw r0, 0xc(r1)
bl fn_8025F648
cmpwi r3, 0x0
beq @80074BE0
li r3, 0x6
b @8007537C
@80074BE0
lis r4, 0x8000
lis r3, 0x1062
lwz r0, 0xf8(r4)
addi r3, r3, 0x4dd3
srwi r0, r0, 2
mulhwu r0, r3, r0
srwi r0, r0, 6
mulli r16, r0, 0x64
bl OSGetTick
lis r4, lbl_803B6E08@ha
mr r19, r3
slwi r18, r31, 2
addi r17, r4, lbl_803B6E08@l
@80074C14
bl OSGetTick
subf r0, r19, r3
cmplw r0, r16
ble @80074C2C
li r3, 0x7
b @8007537C
@80074C2C
mr r3, r31
addi r4, r1, 0x8
bl fn_8025F3F4
cmpwi r3, 0x0
beq @80074C48
li r3, 0x8
b @8007537C
@80074C48
lbz r3, 0x8(r1)
rlwinm r0, r3, 0, 26, 27
cmpwi r0, 0x30
beq @80074C60
li r3, 0x9
b @8007537C
@80074C60
rlwinm r0, r3, 0, 30, 30
cmpwi r0, 0x0
beq @80074C9C
lwz r12, 0x0(r29)
cmplwi r12, 0x0
beq @80074C88
mr r3, r31
lwz r4, 0x0(r28)
mtctr r12
bctrl
@80074C88
lwzx r0, r17, r18
cmpwi r0, 0x0
beq @80074C14
li r3, 0x3e8
b @8007537C
@80074C9C
lis r3, 0x6177
xor r5, r20, r21
addi r0, r3, 0x614b
lis r3, lbl_803B6E08@ha
mullw r4, r5, r0
mr r24, r5
slwi r17, r31, 2
addi r18, r3, lbl_803B6E08@l
li r25, 0x30
li r23, 0x0
addi r26, r4, 0x1
lis r3, 0x1062
lis r20, 0x8000
addi r19, r3, 0x4dd3
b @80074F34
@80074CD8
lwz r5, 0x0(r22)
cmplwi r23, 0xa0
srwi r3, r5, 24
rlwinm r0, r5, 24, 16, 23
rlwinm r4, r5, 8, 8, 15
slwi r5, r5, 24
or r0, r3, r0
or r0, r4, r0
or r0, r5, r0
mr r5, r0
blt @80074E18
subf r5, r24, r0
xor r4, r24, r0
xor r5, r5, r26
li r3, 0x20
li r0, 0x4
mtctr r0
@80074D1C
clrlwi r0, r4, 31
cmplwi r0, 0x0
beq @80074D34
srwi r4, r4, 1
xori r4, r4, 0xa1c1
b @80074D38
@80074D34
srwi r4, r4, 1
@80074D38
clrlwi r0, r4, 31
cmplwi r0, 0x0
beq @80074D50
srwi r4, r4, 1
xori r4, r4, 0xa1c1
b @80074D54
@80074D50
srwi r4, r4, 1
@80074D54
clrlwi r0, r4, 31
cmplwi r0, 0x0
beq @80074D6C
srwi r4, r4, 1
xori r4, r4, 0xa1c1
b @80074D70
@80074D6C
srwi r4, r4, 1
@80074D70
clrlwi r0, r4, 31
cmplwi r0, 0x0
beq @80074D88
srwi r4, r4, 1
xori r4, r4, 0xa1c1
b @80074D8C
@80074D88
srwi r4, r4, 1
@80074D8C
clrlwi r0, r4, 31
cmplwi r0, 0x0
beq @80074DA4
srwi r4, r4, 1
xori r4, r4, 0xa1c1
b @80074DA8
@80074DA4
srwi r4, r4, 1
@80074DA8
clrlwi r0, r4, 31
cmplwi r0, 0x0
beq @80074DC0
srwi r4, r4, 1
xori r4, r4, 0xa1c1
b @80074DC4
@80074DC0
srwi r4, r4, 1
@80074DC4
clrlwi r0, r4, 31
cmplwi r0, 0x0
beq @80074DDC
srwi r4, r4, 1
xori r4, r4, 0xa1c1
b @80074DE0
@80074DDC
srwi r4, r4, 1
@80074DE0
clrlwi r0, r4, 31
cmplwi r0, 0x0
beq @80074DF8
srwi r4, r4, 1
xori r4, r4, 0xa1c1
b @80074DFC
@80074DF8
srwi r4, r4, 1
@80074DFC
subi r3, r3, 0x7
bdnz @80074D1C
lis r3, 0x6177
mr r24, r4
addi r0, r3, 0x614b
mullw r3, r26, r0
addi r26, r3, 0x1
@80074E18
srwi r3, r5, 24
rlwinm r0, r5, 24, 16, 23
rlwinm r4, r5, 8, 8, 15
slwi r5, r5, 24
or r0, r3, r0
mr r3, r31
or r0, r4, r0
addi r4, r1, 0xc
or r0, r5, r0
addi r5, r1, 0x8
stw r0, 0xc(r1)
bl fn_8025F648
cmpwi r3, 0x0
beq @80074E58
li r3, 0xa
b @8007537C
@80074E58
lbz r0, 0x8(r1)
rlwinm r0, r0, 0, 26, 27
cmplw r0, r25
beq @80074E70
li r3, 0x12
b @8007537C
@80074E70
lwz r0, 0xf8(r20)
xori r25, r25, 0x10
srwi r0, r0, 2
mulhwu r0, r19, r0
srwi r0, r0, 6
mulli r16, r0, 0x64
bl OSGetTick
mr r21, r3
@80074E90
bl OSGetTick
subf r0, r21, r3
cmplw r0, r16
ble @80074EA8
li r3, 0xb
b @8007537C
@80074EA8
mr r3, r31
addi r4, r1, 0x8
bl fn_8025F3F4
cmpwi r3, 0x0
beq @80074EC4
li r3, 0xc
b @8007537C
@80074EC4
lbz r3, 0x8(r1)
rlwinm r0, r3, 0, 26, 26
cmpwi r0, 0x0
bne @80074EDC
li r3, 0xd
b @8007537C
@80074EDC
rlwinm r0, r3, 0, 30, 30
cmpwi r0, 0x0
beq @80074F18
lwz r12, 0x0(r29)
cmplwi r12, 0x0
beq @80074F04
mr r3, r31
lwz r4, 0x0(r28)
mtctr r12
bctrl
@80074F04
lwzx r0, r18, r17
cmpwi r0, 0x0
beq @80074E90
li r3, 0x3e8
b @8007537C
@80074F18
lwzx r0, r18, r17
cmpwi r0, 0x0
beq @80074F2C
li r3, 0x3e8
b @8007537C
@80074F2C
addi r23, r23, 0x4
addi r22, r22, 0x4
@80074F34
cmplw r23, r27
blt @80074CD8
lis r3, 0x8000
lwz r0, 0xf8(r3)
srwi r0, r0, 2
mulli r17, r0, 0xa
bl OSGetTick
lis r4, lbl_803B6E08@ha
mr r19, r3
slwi r18, r31, 2
addi r16, r4, lbl_803B6E08@l
@80074F60
bl OSGetTick
subf r0, r19, r3
cmplw r0, r17
ble @80074F78
li r3, 0xe
b @8007537C
@80074F78
mr r3, r31
addi r4, r1, 0x8
bl fn_8025F3F4
cmpwi r3, 0x0
beq @80074F94
li r3, 0xf
b @8007537C
@80074F94
lbz r3, 0x8(r1)
rlwinm r0, r3, 0, 26, 27
cmpwi r0, 0x30
beq @80074FAC
li r3, 0x10
b @8007537C
@80074FAC
andi. r0, r3, 0xa
cmpwi r0, 0x8
bne @800752CC
mr r3, r31
addi r4, r1, 0x10
addi r5, r1, 0x8
bl fn_8025F584
cmpwi r3, 0x0
beq @80074FD8
li r3, 0x11
b @8007537C
@80074FD8
lwz r4, 0x10(r1)
srwi r5, r4, 24
cmplwi r5, 0xff
bne @800752A8
rlwinm r0, r4, 24, 16, 23
rlwinm r3, r4, 8, 8, 15
or r0, r5, r0
slwi r4, r4, 24
or r0, r3, r0
mr r5, r24
or r16, r4, r0
addi r3, r30, 0xcc
mr r4, r16
crclr 6
bl OSReport
srwi r7, r16, 8
li r3, 0x0
li r4, -0x1
@80075020
xor r6, r4, r24
li r5, 0x20
li r0, 0x4
mtctr r0
@80075030
clrlwi r0, r6, 31
cmplwi r0, 0x0
beq @80075048
srwi r6, r6, 1
xori r6, r6, 0xa1c1
b @8007504C
@80075048
srwi r6, r6, 1
@8007504C
clrlwi r0, r6, 31
cmplwi r0, 0x0
beq @80075064
srwi r6, r6, 1
xori r6, r6, 0xa1c1
b @80075068
@80075064
srwi r6, r6, 1
@80075068
clrlwi r0, r6, 31
cmplwi r0, 0x0
beq @80075080
srwi r6, r6, 1
xori r6, r6, 0xa1c1
b @80075084
@80075080
srwi r6, r6, 1
@80075084
clrlwi r0, r6, 31
cmplwi r0, 0x0
beq @8007509C
srwi r6, r6, 1
xori r6, r6, 0xa1c1
b @800750A0
@8007509C
srwi r6, r6, 1
@800750A0
clrlwi r0, r6, 31
cmplwi r0, 0x0
beq @800750B8
srwi r6, r6, 1
xori r6, r6, 0xa1c1
b @800750BC
@800750B8
srwi r6, r6, 1
@800750BC
clrlwi r0, r6, 31
cmplwi r0, 0x0
beq @800750D4
srwi r6, r6, 1
xori r6, r6, 0xa1c1
b @800750D8
@800750D4
srwi r6, r6, 1
@800750D8
clrlwi r0, r6, 31
cmplwi r0, 0x0
beq @800750F0
srwi r6, r6, 1
xori r6, r6, 0xa1c1
b @800750F4
@800750F0
srwi r6, r6, 1
@800750F4
clrlwi r0, r6, 31
cmplwi r0, 0x0
beq @8007510C
srwi r6, r6, 1
xori r6, r6, 0xa1c1
b @80075110
@8007510C
srwi r6, r6, 1
@80075110
subi r5, r5, 0x7
bdnz @80075030
cmplw r7, r6
bne @80075230
xori r5, r4, 0xbb
li r3, 0x20
li r0, 0x4
mtctr r0
@80075130
clrlwi r0, r5, 31
cmplwi r0, 0x0
beq @80075148
srwi r5, r5, 1
xori r5, r5, 0xa1c1
b @8007514C
@80075148
srwi r5, r5, 1
@8007514C
clrlwi r0, r5, 31
cmplwi r0, 0x0
beq @80075164
srwi r5, r5, 1
xori r5, r5, 0xa1c1
b @80075168
@80075164
srwi r5, r5, 1
@80075168
clrlwi r0, r5, 31
cmplwi r0, 0x0
beq @80075180
srwi r5, r5, 1
xori r5, r5, 0xa1c1
b @80075184
@80075180
srwi r5, r5, 1
@80075184
clrlwi r0, r5, 31
cmplwi r0, 0x0
beq @8007519C
srwi r5, r5, 1
xori r5, r5, 0xa1c1
b @800751A0
@8007519C
srwi r5, r5, 1
@800751A0
clrlwi r0, r5, 31
cmplwi r0, 0x0
beq @800751B8
srwi r5, r5, 1
xori r5, r5, 0xa1c1
b @800751BC
@800751B8
srwi r5, r5, 1
@800751BC
clrlwi r0, r5, 31
cmplwi r0, 0x0
beq @800751D4
srwi r5, r5, 1
xori r5, r5, 0xa1c1
b @800751D8
@800751D4
srwi r5, r5, 1
@800751D8
clrlwi r0, r5, 31
cmplwi r0, 0x0
beq @800751F0
srwi r5, r5, 1
xori r5, r5, 0xa1c1
b @800751F4
@800751F0
srwi r5, r5, 1
@800751F4
clrlwi r0, r5, 31
cmplwi r0, 0x0
beq @8007520C
srwi r5, r5, 1
xori r5, r5, 0xa1c1
b @80075210
@8007520C
srwi r5, r5, 1
@80075210
subi r3, r3, 0x7
bdnz @80075130
oris r16, r5, 0xbb00
addi r3, r30, 0x100
mr r5, r16
crclr 6
bl OSReport
b @80075258
@80075230
addis r4, r4, 0x100
addi r3, r3, 0x1
cmplwi r3, 0x100
blt @80075020
mr r4, r16
mr r5, r24
addi r3, r30, 0x130
crclr 6
bl OSReport
li r16, 0x0
@80075258
cmplwi r16, 0x0
bne @80075268
li r3, 0x12
b @8007537C
@80075268
srwi r3, r16, 24
rlwinm r0, r16, 24, 16, 23
rlwinm r4, r16, 8, 8, 15
slwi r5, r16, 24
or r0, r3, r0
mr r3, r31
or r0, r4, r0
addi r4, r1, 0xc
or r0, r5, r0
addi r5, r1, 0x8
stw r0, 0xc(r1)
bl fn_8025F648
cmpwi r3, 0x0
beq @800752FC
li r3, 0x13
b @8007537C
@800752A8
cmplwi r5, 0xcc
beq @800752B8
li r3, 0x14
b @8007537C
@800752B8
lwzx r0, r16, r18
cmpwi r0, 0x0
beq @80074F60
li r3, 0x3e8
b @8007537C
@800752CC
lwz r12, 0x0(r29)
cmplwi r12, 0x0
beq @800752E8
mr r3, r31
lwz r4, 0x0(r28)
mtctr r12
bctrl
@800752E8
lwzx r0, r16, r18
cmpwi r0, 0x0
beq @80074F60
li r3, 0x3e8
b @8007537C
@800752FC
lis r3, 0x8000
lwz r0, 0xf8(r3)
srwi r19, r0, 2
bl OSGetTick
lis r4, lbl_803B6E08@ha
mr r18, r3
slwi r17, r31, 2
addi r16, r4, lbl_803B6E08@l
@8007531C
bl OSGetTick
subf r0, r18, r3
cmplw r0, r19
ble @80075334
li r3, 0x15
b @8007537C
@80075334
mr r3, r31
bl fn_800D0F44
subis r0, r3, 0x4
cmplwi r0, 0x0
bne @80075378
lwz r12, 0x0(r29)
cmplwi r12, 0x0
beq @80075364
mr r3, r31
lwz r4, 0x0(r28)
mtctr r12
bctrl
@80075364
lwzx r0, r16, r17
cmpwi r0, 0x0
beq @8007531C
li r3, 0x3e8
b @8007537C
@80075378
li r3, 0x0
@8007537C
lmw r16, 0x20(r1)
lwz r0, 0x64(r1)
mtlr r0
addi r1, r1, 0x60
blr
