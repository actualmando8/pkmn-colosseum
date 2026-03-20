stwu r1, -0x8d0(r1)
mflr r0
stw r0, 0x8d4(r1)
stmw r14, 0x888(r1)
stw r3, 0x8(r1)
mr r15, r4
mr r17, r5
stw r6, 0xc(r1)
li r3, 0x0
li r4, 0x0
li r5, 0x14
li r6, 0x0
bl fn_801F54A4
clrlwi r24, r3, 16
mr r3, r24
bl fn_8020E204
mr r18, r3
bl fn_8020E1D4
mr r19, r3
mr r3, r18
bl fn_8020E1BC
mr r16, r3
mr r3, r18
bl fn_8020E1A4
mr r18, r3
clrlwi r3, r19, 24
clrlwi r0, r18, 24
mr r4, r15
mullw r0, r3, r0
mr r5, r24
li r3, 0xb
slwi r0, r0, 1
clrlwi r23, r0, 24
bl fn_801F02AC
li r4, 0x0
mr r20, r3
bl fn_801F981C
clrlwi r0, r19, 24
li r4, 0x1
cmplwi r0, 0x2
stw r3, 0x28(r1)
stb r4, 0x24(r1)
bne @8008A0AC
mr r4, r20
mr r5, r24
li r3, 0x7
bl fn_801F02AC
li r4, 0x0
mr r19, r3
bl fn_801F981C
stw r3, 0x2c(r1)
mr r3, r19
bl fn_801FCDB4
stb r3, 0x25(r1)
mr r4, r20
mr r5, r24
li r3, 0x9
bl fn_801F02AC
li r4, 0x0
mr r19, r3
bl fn_801F981C
stw r3, 0x30(r1)
mr r3, r19
bl fn_801FCDB4
stb r3, 0x26(r1)
mr r4, r20
mr r5, r24
li r3, 0xa
bl fn_801F02AC
li r4, 0x0
mr r19, r3
bl fn_801F981C
stw r3, 0x34(r1)
mr r3, r19
bl fn_801FCDB4
stb r3, 0x27(r1)
b @8008A134
@8008A0AC
clrlwi r0, r18, 24
cmplwi r0, 0x2
bne @8008A110
mr r3, r20
li r4, 0x1
bl fn_801F981C
stw r3, 0x2c(r1)
mr r4, r20
mr r5, r24
li r3, 0x9
bl fn_801F02AC
li r4, 0x0
mr r19, r3
bl fn_801F981C
stw r3, 0x30(r1)
mr r3, r19
li r4, 0x1
bl fn_801F981C
li r0, 0x2
li r4, 0x1
stw r3, 0x34(r1)
stb r4, 0x25(r1)
stb r0, 0x26(r1)
stb r0, 0x27(r1)
b @8008A134
@8008A110
mr r4, r20
mr r5, r24
li r3, 0x9
bl fn_801F02AC
li r4, 0x0
bl fn_801F981C
li r0, 0x1
stw r3, 0x2c(r1)
stb r0, 0x25(r1)
@8008A134
mr r3, r15
clrlwi r4, r17, 16
bl fn_801F981C
clrlwi r0, r18, 24
mr r22, r3
cmplwi r0, 0x2
bne @8008A158
addi r0, r17, 0x1
b @8008A15C
@8008A158
li r0, 0x0
@8008A15C
clrlwi r0, r0, 24
lbz r4, 0x3b(r1)
rlwimi r4, r0, 0, 25, 31
lwz r0, 0xc(r1)
addi r25, r1, 0x38
stb r4, 0x3b(r1)
clrlwi r3, r0, 24
clrlwi r0, r4, 24
rlwimi r0, r3, 7, 24, 24
stb r23, 0x3f(r1)
mr r27, r25
clrlwi r26, r16, 24
stb r0, 0x3b(r1)
li r19, 0x0
li r18, 0x0
li r21, 0x0
li r28, 0x8
b @8008A364
@8008A1A4
mr r3, r15
clrlwi r4, r21, 16
bl fn_801F986C
mr r29, r3
cmplwi r29, 0x0
beq @8008A36C
bl fn_80205BE8
mr r17, r3
bl fn_8011E7C0
clrlwi r0, r3, 16
mr r3, r15
slw r0, r0, r28
mr r4, r29
or r19, r19, r0
bl fn_801F8C00
clrlwi r0, r3, 24
mr r3, r17
slw r0, r0, r28
or r18, r18, r0
bl fn_8011E8F4
mr r16, r3
mr r3, r17
bl fn_8011F188
clrlwi r0, r3, 16
mr r3, r17
slwi r4, r0, 8
srawi r0, r0, 8
or r0, r4, r0
clrlwi r0, r0, 16
sth r0, 0x24(r27)
bl fn_8008C5D4
clrlwi r20, r3, 16
mr r3, r29
clrlwi r0, r20, 24
stb r0, 0x26(r27)
bl fn_802042E0
clrlwi r5, r3, 16
lbz r0, 0x27(r27)
neg r4, r5
mr r3, r17
or r4, r4, r5
srwi r4, r4, 31
rlwimi r0, r4, 7, 24, 24
stb r0, 0x27(r27)
bl fn_8011E808
clrlwi r4, r3, 24
clrlwi r7, r16, 24
subfic r3, r4, 0xff
lbz r6, 0x27(r27)
subi r0, r4, 0xff
clrlwi r5, r7, 28
or r0, r3, r0
rlwinm r3, r7, 0, 24, 27
srwi r0, r0, 31
neg r4, r5
rlwimi r6, r0, 6, 25, 25
neg r0, r3
stb r6, 0x27(r27)
or r4, r4, r5
srwi r5, r4, 31
or r3, r0, r3
lbz r4, 0x27(r27)
rlwimi r4, r5, 4, 27, 27
srawi r0, r20, 8
srwi r5, r3, 31
stb r4, 0x27(r27)
clrlwi r4, r0, 24
mr r3, r29
lbz r0, 0x27(r27)
rlwimi r0, r5, 5, 26, 26
stb r0, 0x27(r27)
lbz r0, 0x27(r27)
rlwimi r0, r4, 0, 28, 31
stb r0, 0x27(r27)
bl fn_802042E0
clrlwi r0, r3, 16
mr r31, r27
slwi r3, r0, 8
mr r30, r27
srawi r0, r0, 8
mr r29, r27
or r0, r3, r0
li r20, 0x0
clrlwi r0, r0, 16
sth r0, 0x28(r27)
@8008A2F8
mr r3, r17
clrlwi r4, r20, 16
bl fn_8011F228
mr r0, r3
mr r3, r17
mr r16, r0
clrlwi r4, r20, 16
clrlwi r0, r16, 16
slwi r5, r0, 8
srawi r0, r0, 8
or r0, r5, r0
clrlwi r0, r0, 16
sth r0, 0x2c(r31)
bl fn_8011F1F0
stb r3, 0x34(r30)
mr r4, r16
addi r3, r29, 0x38
bl fn_80083ECC
addi r31, r31, 0x2
addi r30, r30, 0x1
addi r29, r29, 0x50
addi r20, r20, 0x1
cmpwi r20, 0x4
blt @8008A2F8
addi r28, r28, 0x4
addi r27, r27, 0x154
addi r21, r21, 0x1
@8008A364
cmpw r21, r26
blt @8008A1A4
@8008A36C
clrlwi r20, r21, 24
rlwinm r0, r18, 0, 16, 23
slwi r3, r20, 2
rlwinm r4, r18, 0, 8, 15
addi r3, r3, 0x8
li r5, 0x1
slw r5, r5, r3
slwi r3, r18, 24
neg r5, r5
slwi r0, r0, 8
or r19, r19, r5
srwi r4, r4, 8
rlwinm r5, r19, 0, 16, 23
or r0, r3, r0
rlwinm r3, r19, 0, 8, 15
slwi r6, r19, 24
slwi r5, r5, 8
srwi r8, r19, 24
srwi r7, r3, 8
srwi r3, r18, 24
or r5, r6, r5
or r0, r4, r0
or r5, r7, r5
lwz r4, 0x38(r1)
or r5, r8, r5
or r3, r3, r0
lwz r0, 0x3c(r1)
rlwimi r4, r5, 8, 0, 23
rlwimi r0, r3, 8, 0, 23
stw r4, 0x38(r1)
mr r3, r22
li r26, 0x0
stw r0, 0x3c(r1)
bl fn_80205B8C
mr r0, r3
mr r3, r22
mr r21, r0
addi r4, r1, 0x20
addi r5, r1, 0x1c
bl fn_801FDB78
lwz r9, 0x20(r1)
mr r3, r21
lwz r10, 0x1c(r1)
rlwinm r4, r9, 0, 16, 23
rlwinm r8, r9, 0, 8, 15
rlwinm r0, r10, 0, 16, 23
rlwinm r5, r10, 0, 8, 15
slwi r7, r9, 24
slwi r6, r4, 8
slwi r4, r10, 24
slwi r0, r0, 8
srwi r8, r8, 8
or r6, r7, r6
srwi r5, r5, 8
or r0, r4, r0
srwi r7, r9, 24
or r4, r8, r6
or r6, r7, r4
srwi r4, r10, 24
or r0, r5, r0
stw r6, 0x40(r1)
or r0, r4, r0
stw r0, 0x44(r1)
bl fn_8011F5C8
clrlwi r4, r3, 16
slwi r3, r4, 8
srawi r0, r4, 8
cmplwi r4, 0x181
or r0, r3, r0
clrlwi r0, r0, 16
sth r0, 0x48(r1)
bne @8008A528
mr r3, r22
li r4, 0x0
bl fn_801FD614
clrlwi r0, r3, 16
cmplwi r0, 0xa
bne @8008A4AC
li r4, 0x1
b @8008A4D0
@8008A4AC
cmplwi r0, 0xb
bne @8008A4BC
li r4, 0x2
b @8008A4D0
@8008A4BC
cmplwi r0, 0xf
bne @8008A4CC
li r4, 0x3
b @8008A4D0
@8008A4CC
li r4, 0x0
@8008A4D0
clrlwi r0, r3, 16
cmplwi r0, 0xa
bne @8008A4E4
li r0, 0x1
b @8008A508
@8008A4E4
cmplwi r0, 0xb
bne @8008A4F4
li r0, 0x2
b @8008A508
@8008A4F4
cmplwi r0, 0xf
bne @8008A504
li r0, 0x3
b @8008A508
@8008A504
li r0, 0x0
@8008A508
clrlwi r3, r0, 16
clrlwi r0, r4, 16
slwi r3, r3, 8
srawi r0, r0, 8
or r0, r3, r0
clrlwi r0, r0, 16
sth r0, 0x4a(r1)
b @8008A530
@8008A528
li r0, 0x0
sth r0, 0x4a(r1)
@8008A530
lwz r0, 0xc(r1)
cmpwi r0, 0x0
beq @8008A548
li r3, 0x0
li r6, 0x0
b @8008A5E0
@8008A548
bl fn_801F6B48
mr r4, r22
addi r5, r1, 0x18
bl fn_801F2020
clrlwi r0, r3, 24
cmplwi r0, 0x2
bne @8008A5A8
lwz r3, 0x18(r1)
bl fn_80207BF4
clrlwi r0, r3, 16
cmplwi r0, 0x17
bne @8008A580
li r0, 0x2
b @8008A5A4
@8008A580
cmplwi r0, 0x2a
bne @8008A590
li r0, 0x3
b @8008A5A4
@8008A590
cmplwi r0, 0x47
bne @8008A5A0
li r0, 0x4
b @8008A5A4
@8008A5A0
li r0, 0x0
@8008A5A4
clrlwi r3, r0, 24
@8008A5A8
subi r6, r23, 0x1
addi r4, r1, 0x28
slwi r0, r6, 2
lwz r5, 0x18(r1)
add r4, r4, r0
mtctr r6
cmpwi r6, 0x0
ble @8008A5E0
@8008A5C8
lwz r0, 0x0(r4)
cmplw r5, r0
beq @8008A5E0
subi r4, r4, 0x4
subi r6, r6, 0x1
bdnz @8008A5C8
@8008A5E0
lbz r0, 0x5b(r1)
rlwimi r0, r3, 0, 28, 31
clrlwi r4, r6, 24
mr r3, r22
stb r0, 0x5b(r1)
clrlwi r0, r0, 24
rlwimi r0, r4, 4, 24, 27
li r4, 0x0
stb r0, 0x5b(r1)
bl fn_801FF1BC
lbz r0, 0x5a(r1)
rlwimi r0, r3, 0, 28, 31
li r19, 0x0
mr r16, r25
stb r0, 0x5a(r1)
mr r17, r25
mr r18, r19
lis r3, fn_8008A99C@ha
addi r15, r3, fn_8008A99C@l
@8008A62C
li r0, 0x0
mr r3, r21
stb r0, lbl_8047A678@sda21(r0)
clrlwi r4, r19, 16
bl fn_8011F228
mr r0, r3
mr r3, r21
mr r27, r0
clrlwi r4, r19, 16
clrlwi r0, r27, 16
slwi r5, r0, 8
srawi r0, r0, 8
or r0, r5, r0
clrlwi r0, r0, 16
sth r0, 0x14(r16)
bl fn_8011F1F0
stb r3, 0x1c(r17)
mr r3, r22
mr r4, r27
mr r5, r24
mr r6, r15
li r7, 0x1
li r8, 0x0
li r9, -0x1
bl fn_8022B2CC
lbz r0, lbl_8047A678@sda21(r0)
cmplwi r0, 0x0
beq @8008A6AC
li r0, 0x8
slw r0, r0, r18
or r0, r26, r0
clrlwi r26, r0, 16
@8008A6AC
mr r3, r22
clrlwi r4, r19, 16
addi r6, r1, 0x10
li r5, 0x1
bl fn_801FFEC8
clrlwi r3, r3, 24
slw r0, r3, r18
or r0, r26, r0
cmplwi r3, 0x5
clrlwi r26, r0, 16
bne @8008A6DC
lhz r14, 0x10(r1)
@8008A6DC
addi r16, r16, 0x2
addi r17, r17, 0x1
addi r18, r18, 0x4
addi r19, r19, 0x1
cmpwi r19, 0x4
blt @8008A62C
srawi r0, r26, 8
slwi r3, r26, 8
or r0, r3, r0
slwi r3, r14, 8
clrlwi r4, r0, 16
srawi r0, r14, 8
sth r4, 0x58(r1)
or r0, r3, r0
li r4, 0x3
clrlwi r0, r0, 16
lhz r3, 0x52(r1)
cmplw r3, r0
beq @8008A74C
li r4, 0x2
lhz r3, 0x50(r1)
cmplw r3, r0
beq @8008A74C
li r4, 0x1
lhz r3, 0x4e(r1)
cmplw r3, r0
beq @8008A74C
li r4, 0x0
@8008A74C
clrlwi r3, r4, 24
lbz r0, 0x5a(r1)
rlwimi r0, r3, 4, 24, 27
addi r14, r1, 0x28
stb r0, 0x5a(r1)
addi r15, r1, 0x24
li r16, 0x0
b @8008A898
@8008A76C
addi r3, r25, 0x81c
li r4, 0x0
li r5, 0xc
bl memset
lwz r3, 0x0(r14)
cmplw r3, r22
bne @8008A794
li r0, 0xff
stb r0, 0x81c(r25)
b @8008A888
@8008A794
bl fn_80205B8C
mr r18, r3
bl fn_8011F598
mr r17, r3
bl fn_80135A70
clrlwi r19, r3, 24
mr r3, r18
bl fn_8011F4F0
mr r4, r3
mr r5, r19
addi r3, r25, 0x81c
bl fn_800F9AEC
cmpwi r3, 0xa
bge @8008A7D8
addi r0, r3, 0x81c
li r3, 0xff
stbx r3, r25, r0
@8008A7D8
mr r3, r18
bl fn_8011F5C8
mr r19, r3
mr r3, r18
bl fn_801231A4
clrlwi r5, r19, 16
lbz r6, 0x826(r25)
rlwimi r6, r3, 6, 24, 25
lbz r4, 0x0(r15)
subfic r0, r5, 0x20
stb r6, 0x826(r25)
cntlzw r3, r0
subfic r0, r5, 0x1d
srwi r3, r3, 5
lbz r5, 0x826(r25)
clrlwi r3, r3, 24
cntlzw r0, r0
rlwimi r5, r3, 5, 26, 26
mr r3, r17
stb r5, 0x826(r25)
srwi r0, r0, 5
clrlwi r5, r0, 24
lbz r0, 0x826(r25)
rlwimi r0, r5, 4, 27, 27
stb r0, 0x826(r25)
lbz r0, 0x826(r25)
rlwimi r0, r4, 0, 29, 31
stb r0, 0x826(r25)
bl fn_80135A70
clrlwi r0, r3, 24
cmplwi r0, 0x1
bne @8008A86C
lbz r0, 0x826(r25)
li r3, 0x1
rlwimi r0, r3, 3, 28, 28
stb r0, 0x826(r25)
b @8008A87C
@8008A86C
lbz r0, 0x826(r25)
li r3, 0x0
rlwimi r0, r3, 3, 28, 28
stb r0, 0x826(r25)
@8008A87C
mr r3, r18
bl fn_8011F4A8
stb r3, 0x827(r25)
@8008A888
addi r25, r25, 0xc
addi r14, r14, 0x4
addi r15, r15, 0x1
addi r16, r16, 0x1
@8008A898
cmpw r16, r23
blt @8008A76C
mulli r3, r20, 0x154
addi r0, r1, 0x38
addi r4, r1, 0x854
mulli r14, r23, 0xc
addi r3, r3, 0x24
mr r5, r14
add r3, r0, r3
bl fn_800C8174
mulli r0, r20, 0x154
lwz r3, 0x8(r1)
addi r4, r1, 0x38
subi r15, r3, 0x1
add r5, r0, r14
mr r3, r15
addi r5, r5, 0x24
bl fn_80072D58
@8008A8E0
bl fn_801EF634
clrlwi r0, r3, 16
cmplwi r0, 0x1
bne @8008A900
mr r3, r15
bl fn_80072A00
lis r3, 0x5
b @8008A988
@8008A900
li r3, 0x0
bl fn_801F1700
clrlwi r0, r3, 24
cmplwi r0, 0x1
bne @8008A934
bl fn_80265924
clrlwi r0, r3, 24
cmplwi r0, 0x1
bne @8008A934
mr r3, r15
bl fn_80072A00
lis r3, 0x4
b @8008A988
@8008A934
mr r3, r15
addi r4, r1, 0x14
bl fn_80072C74
cmpwi r3, 0x0
ble @8008A950
oris r3, r3, 0x5
b @8008A988
@8008A950
bne @8008A980
lwz r5, 0x14(r1)
rlwinm r0, r5, 0, 16, 23
rlwinm r4, r5, 0, 8, 15
slwi r3, r5, 24
srwi r5, r5, 24
slwi r0, r0, 8
srwi r4, r4, 8
or r0, r3, r0
or r0, r4, r0
or r3, r5, r0
b @8008A988
@8008A980
bl fn_800F0308
b @8008A8E0
@8008A988
lmw r14, 0x888(r1)
lwz r0, 0x8d4(r1)
mtlr r0
addi r1, r1, 0x8d0
blr
