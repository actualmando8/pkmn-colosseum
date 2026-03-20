stwu r1, -0x40(r1)
mflr r0
stw r0, 0x44(r1)
stmw r21, 0x14(r1)
mr r23, r3
mr r24, r4
li r31, 0x0
li r4, 0x0
bl fn_801040D0
mr r0, r3
mr r3, r23
mr r30, r0
li r4, 0x1
bl fn_801040D0
mr r21, r3
mr r3, r23
li r4, 0x2
bl fn_801040D0
mr r0, r3
mr r3, r23
mr r29, r0
li r4, 0x3
bl fn_801040D0
lbz r0, 0x1(r23)
mr r22, r3
li r28, 0x0
li r27, 0x0
extsb r0, r0
li r26, 0x0
cmpwi r0, 0x4
li r25, 0x0
bge @8006CD50
cmpwi r0, 0x0
beq @8006D53C
bge @8006CD58
b @8006D53C
@8006CD50
cmpwi r0, 0x6
b @8006D53C
@8006CD58
lha r0, 0x6(r24)
cmpwi r0, 0xf2c
beq @8006D108
bge @8006CE2C
cmpwi r0, 0xe79
beq @8006D0B4
bge @8006CDD0
cmpwi r0, 0xe61
beq @8006D06C
bge @8006CDA8
cmpwi r0, 0xe5e
beq @8006D048
bge @8006CD9C
cmpwi r0, 0xe5c
beq @8006D030
bge @8006D03C
b @8006D2C8
@8006CD9C
cmpwi r0, 0xe60
bge @8006D060
b @8006D054
@8006CDA8
cmpwi r0, 0xe76
beq @8006D090
bge @8006CDC4
cmpwi r0, 0xe74
beq @8006D078
bge @8006D084
b @8006D2C8
@8006CDC4
cmpwi r0, 0xe78
bge @8006D0A8
b @8006D09C
@8006CDD0
cmpwi r0, 0xe80
beq @8006CF24
bge @8006CE04
cmpwi r0, 0xe7d
beq @8006D0E4
bge @8006CDF8
cmpwi r0, 0xe7b
beq @8006D0CC
bge @8006D0D8
b @8006D0C0
@8006CDF8
cmpwi r0, 0xe7f
bge @8006D0FC
b @8006D0F0
@8006CE04
cmpwi r0, 0xe84
beq @8006D1E0
bge @8006CE20
cmpwi r0, 0xe82
beq @8006D2C8
bge @8006CEE0
b @8006CFA4
@8006CE20
cmpwi r0, 0xe86
bge @8006D2C8
b @8006D024
@8006CE2C
cmpwi r0, 0xf4a
beq @8006D198
bge @8006CE90
cmpwi r0, 0xf44
beq @8006D150
bge @8006CE6C
cmpwi r0, 0xf30
beq @8006D138
bge @8006CE60
cmpwi r0, 0xf2e
beq @8006D120
bge @8006D12C
b @8006D114
@8006CE60
cmpwi r0, 0xf32
bge @8006D2C8
b @8006D144
@8006CE6C
cmpwi r0, 0xf47
beq @8006D174
bge @8006CE84
cmpwi r0, 0xf46
bge @8006D168
b @8006D15C
@8006CE84
cmpwi r0, 0xf49
bge @8006D18C
b @8006D180
@8006CE90
cmpwi r0, 0xfc9
beq @8006CEE0
bge @8006CEC4
cmpwi r0, 0xf4e
beq @8006D1C8
bge @8006CEB8
cmpwi r0, 0xf4c
beq @8006D1B0
bge @8006D1BC
b @8006D1A4
@8006CEB8
cmpwi r0, 0xf50
bge @8006D2C8
b @8006D1D4
@8006CEC4
cmpwi r0, 0x12bc
beq @8006D1FC
bge @8006D2C8
cmpwi r0, 0xfcb
beq @8006D024
bge @8006D2C8
b @8006D1E0
@8006CEE0
cmpwi r21, 0x0
beq @8006CF00
bge @8006CEF4
cmpwi r21, -0x1
b @8006CF18
@8006CEF4
cmpwi r21, 0x3
bge @8006CF18
b @8006CF0C
@8006CF00
li r0, 0x3d8d
stw r0, 0x4c(r24)
b @8006D2C8
@8006CF0C
li r0, 0x3d8f
stw r0, 0x4c(r24)
b @8006D2C8
@8006CF18
li r0, 0x0
stw r0, 0x4c(r24)
b @8006D2C8
@8006CF24
cmplwi r22, 0x0
beq @8006D2C8
lwz r0, 0xc(r22)
cmplwi r0, 0x6
bgt @8006D2C8
lis r3, jumptable_802EDFB0@ha
slwi r0, r0, 2
addi r3, r3, jumptable_802EDFB0@l
lwzx r0, r3, r0
mtctr r0
bctr
li r0, 0x3d91
stw r0, 0x4c(r24)
b @8006D2C8
li r0, 0x3d93
stw r0, 0x4c(r24)
b @8006D2C8
li r0, 0x3d94
stw r0, 0x4c(r24)
b @8006D2C8
li r0, 0x3d95
stw r0, 0x4c(r24)
b @8006D2C8
li r0, 0x3d9b
stw r0, 0x4c(r24)
b @8006D2C8
li r0, 0x3d9c
stw r0, 0x4c(r24)
b @8006D2C8
li r0, 0x3daf
stw r0, 0x4c(r24)
b @8006D2C8
@8006CFA4
cmplwi r22, 0x0
beq @8006D2C8
lwz r0, 0x4(r22)
cmpwi r0, 0x1
beq @8006CFD4
bge @8006CFE0
cmpwi r0, 0x0
bge @8006CFC8
b @8006CFE0
@8006CFC8
li r0, 0x3d9d
stw r0, 0x4c(r24)
b @8006CFF4
@8006CFD4
li r0, 0x3d9e
stw r0, 0x4c(r24)
b @8006CFF4
@8006CFE0
lis r3, lbl_80268680@ha
li r4, 0xb47
addi r3, r3, lbl_80268680@l
li r5, lbl_8047C064@sda21
bl fn_80196E10
@8006CFF4
mr r3, r23
li r4, 0xe80
bl fn_801046C8
mr r22, r3
lwz r3, 0x4c(r22)
bl fn_800FA444
lha r0, 0x50(r22)
srwi r3, r3, 16
add r0, r0, r3
extsh r0, r0
sth r0, 0x50(r24)
b @8006D2C8
@8006D024
li r0, 0x3d8b
stw r0, 0x4c(r24)
b @8006D2C8
@8006D030
li r31, 0x1
li r28, 0x0
b @8006D2C8
@8006D03C
li r31, 0x1
li r28, 0x1
b @8006D2C8
@8006D048
li r31, 0x1
li r28, 0x2
b @8006D2C8
@8006D054
li r31, 0x1
li r28, 0x3
b @8006D2C8
@8006D060
li r31, 0x1
li r28, 0x4
b @8006D2C8
@8006D06C
li r31, 0x1
li r28, 0x5
b @8006D2C8
@8006D078
li r31, 0x2
li r28, 0x0
b @8006D2C8
@8006D084
li r31, 0x2
li r28, 0x1
b @8006D2C8
@8006D090
li r31, 0x2
li r28, 0x2
b @8006D2C8
@8006D09C
li r31, 0x2
li r28, 0x3
b @8006D2C8
@8006D0A8
li r31, 0x2
li r28, 0x4
b @8006D2C8
@8006D0B4
li r31, 0x2
li r28, 0x5
b @8006D2C8
@8006D0C0
li r31, 0x3
li r28, 0x0
b @8006D2C8
@8006D0CC
li r31, 0x3
li r28, 0x1
b @8006D2C8
@8006D0D8
li r31, 0x3
li r28, 0x2
b @8006D2C8
@8006D0E4
li r31, 0x3
li r28, 0x3
b @8006D2C8
@8006D0F0
li r31, 0x3
li r28, 0x4
b @8006D2C8
@8006D0FC
li r31, 0x3
li r28, 0x5
b @8006D2C8
@8006D108
li r31, 0x1
li r28, 0x0
b @8006D2C8
@8006D114
li r31, 0x1
li r28, 0x1
b @8006D2C8
@8006D120
li r31, 0x1
li r28, 0x2
b @8006D2C8
@8006D12C
li r31, 0x1
li r28, 0x3
b @8006D2C8
@8006D138
li r31, 0x1
li r28, 0x4
b @8006D2C8
@8006D144
li r31, 0x1
li r28, 0x5
b @8006D2C8
@8006D150
li r31, 0x2
li r28, 0x0
b @8006D2C8
@8006D15C
li r31, 0x2
li r28, 0x1
b @8006D2C8
@8006D168
li r31, 0x2
li r28, 0x2
b @8006D2C8
@8006D174
li r31, 0x2
li r28, 0x3
b @8006D2C8
@8006D180
li r31, 0x2
li r28, 0x4
b @8006D2C8
@8006D18C
li r31, 0x2
li r28, 0x5
b @8006D2C8
@8006D198
li r31, 0x3
li r28, 0x0
b @8006D2C8
@8006D1A4
li r31, 0x3
li r28, 0x1
b @8006D2C8
@8006D1B0
li r31, 0x3
li r28, 0x2
b @8006D2C8
@8006D1BC
li r31, 0x3
li r28, 0x3
b @8006D2C8
@8006D1C8
li r31, 0x3
li r28, 0x4
b @8006D2C8
@8006D1D4
li r31, 0x3
li r28, 0x5
b @8006D2C8
@8006D1E0
cmplwi r30, 0x0
beq @8006D2C8
mr r3, r30
bl fn_8012AC54
li r31, 0x4
mr r25, r3
b @8006D2C8
@8006D1FC
cmplwi r22, 0x0
beq @8006D2C8
lwz r0, 0x0(r22)
li r21, 0x0
cmpwi r0, 0x1
beq @8006D270
bge @8006D280
cmpwi r0, 0x0
bge @8006D224
b @8006D280
@8006D224
lwz r3, 0x14(r22)
cmpwi r3, 0x6
beq @8006D260
bge @8006D240
cmpwi r3, 0x0
bge @8006D24C
b @8006D280
@8006D240
cmpwi r3, 0x8
bge @8006D280
b @8006D268
@8006D24C
addi r4, r3, 0x1
li r3, 0x2f
bl fn_80132A38
li r21, 0x3d9f
b @8006D280
@8006D260
li r21, 0x3da0
b @8006D280
@8006D268
li r21, 0x3da1
b @8006D280
@8006D270
lwz r4, 0x14(r22)
li r3, 0x2f
bl fn_80132A38
li r21, 0x3da2
@8006D280
lwz r5, 0x64(r24)
mr r6, r21
li r3, 0x0
li r4, 0x0
bl fn_800FB680
mr r3, r21
bl fn_800FA444
srwi r3, r3, 16
lwz r4, 0x18(r22)
addi r0, r3, 0x24
li r3, 0x2f
extsh r21, r0
bl fn_80132A38
lwz r5, 0x64(r24)
mr r3, r21
li r4, 0x0
li r6, 0x3da4
bl fn_800FB680
@8006D2C8
cmpwi r31, 0x0
beq @8006D53C
mr r3, r30
mr r4, r28
bl fn_8012AC08
mr r28, r3
bl fn_80077A5C
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8006D2F8
li r28, 0x0
b @8006D328
@8006D2F8
mr r3, r28
bl fn_80123FBC
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8006D31C
li r28, 0x0
li r26, 0x1
li r27, 0x1
b @8006D328
@8006D31C
mr r3, r28
bl fn_8011E8DC
mr r27, r3
@8006D328
cmpwi r31, 0x3
beq @8006D460
bge @8006D344
cmpwi r31, 0x1
beq @8006D350
bge @8006D36C
b @8006D53C
@8006D344
cmpwi r31, 0x5
bge @8006D53C
b @8006D514
@8006D350
cmplwi r28, 0x0
beq @8006D53C
mr r3, r23
mr r4, r24
mr r5, r28
bl fn_8010B718
b @8006D53C
@8006D36C
clrlwi r0, r26, 24
cmplwi r0, 0x0
beq @8006D390
lwz r5, 0x64(r24)
li r3, 0x0
li r4, 0x0
li r6, 0x56c
bl fn_800FB680
b @8006D53C
@8006D390
clrlwi r0, r27, 24
cmplwi r0, 0x0
beq @8006D3B4
lwz r5, 0x64(r24)
li r3, 0x0
li r4, 0x0
li r6, 0x56b
bl fn_800FB680
b @8006D53C
@8006D3B4
cmplwi r28, 0x0
beq @8006D53C
mr r3, r28
bl fn_8011F4F0
cmplwi r3, 0x0
beq @8006D3F8
lhz r0, 0x0(r3)
cmplwi r0, 0x0
beq @8006D3F8
mr r4, r3
li r3, 0x37
bl fn_80132A38
lwz r5, 0x64(r24)
li r3, 0x0
li r4, 0x0
li r6, 0xe7
bl fn_800FB680
@8006D3F8
mr r3, r28
bl fn_8001DA60
clrlwi r0, r3, 24
cmpwi r0, 0x1
beq @8006D42C
bge @8006D41C
cmpwi r0, 0x0
bge @8006D424
b @8006D434
@8006D41C
cmpwi r0, 0x3
b @8006D434
@8006D424
li r6, 0xd67
b @8006D438
@8006D42C
li r6, 0xd68
b @8006D438
@8006D434
li r6, 0x0
@8006D438
cmplwi r6, 0x0
beq @8006D53C
lbz r0, 0x67(r24)
cmplwi r0, 0xff
bne @8006D53C
lwz r5, 0x64(r24)
li r3, 0x5c
li r4, 0x0
bl fn_800FB680
b @8006D53C
@8006D460
cmplwi r28, 0x0
beq @8006D53C
clrlwi r0, r27, 24
cmplwi r0, 0x0
bne @8006D53C
lwz r21, 0x64(r24)
mr r3, r28
li r4, 0x0
li r5, 0x7a
li r6, 0x0
bl fn_8012640C
mr r4, r3
li r3, 0x2f
bl fn_80132A38
cmplwi r29, 0x0
beq @8006D4FC
mr r3, r28
mr r4, r29
li r5, 0x0
bl fn_800774D4
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8006D4F4
mr r3, r28
mr r4, r29
li r5, 0x1
bl fn_800774D4
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8006D4F4
mr r3, r30
mr r4, r29
li r5, 0x0
bl fn_80076F2C
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8006D4FC
@8006D4F4
oris r21, r21, 0xff00
rlwinm r21, r21, 0, 24, 7
@8006D4FC
mr r5, r21
li r3, 0x0
li r4, 0x0
li r6, 0x41fa
bl fn_800FB680
b @8006D53C
@8006D514
cmplwi r25, 0x0
beq @8006D53C
mr r4, r25
li r3, 0x37
bl fn_80132A38
lwz r5, 0x64(r24)
li r3, 0x0
li r4, 0x0
li r6, 0xcf
bl fn_800FB680
@8006D53C
lmw r21, 0x14(r1)
lwz r0, 0x44(r1)
mtlr r0
addi r1, r1, 0x40
blr
