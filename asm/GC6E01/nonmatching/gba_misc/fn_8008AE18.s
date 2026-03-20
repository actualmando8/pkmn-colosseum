stwu r1, -0x40(r1)
mflr r0
stw r0, 0x44(r1)
stmw r26, 0x28(r1)
mr r28, r3
mr r31, r4
mr r3, r4
li r29, 0x0
li r4, 0x0
li r5, 0x64
bl memset
mr r3, r28
bl fn_80123FBC
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8008BBC8
mr r3, r28
bl fn_8011F5B0
rlwinm r0, r3, 0, 16, 23
rlwinm r5, r3, 0, 8, 15
slwi r4, r3, 24
srwi r6, r3, 24
slwi r0, r0, 8
srwi r5, r5, 8
or r0, r4, r0
mr r3, r28
or r0, r5, r0
or r0, r6, r0
stw r0, 0x0(r31)
bl fn_8011F520
rlwinm r0, r3, 0, 16, 23
rlwinm r5, r3, 0, 8, 15
slwi r4, r3, 24
srwi r6, r3, 24
slwi r0, r0, 8
srwi r5, r5, 8
or r0, r4, r0
mr r3, r28
or r0, r5, r0
or r0, r6, r0
stw r0, 0x4(r31)
bl fn_8011F598
mr r30, r3
bl fn_80135AB8
clrlwi r0, r3, 24
cmplwi r0, 0xb
bgt @8008AF64
lis r3, jumptable_802EEBE0@ha
slwi r0, r0, 2
addi r3, r3, jumptable_802EEBE0@l
lwzx r0, r3, r0
mtctr r0
bctr
lhz r0, 0x10(r1)
li r3, 0x1
rlwimi r0, r3, 7, 21, 24
sth r0, 0x10(r1)
b @8008AF74
lhz r0, 0x10(r1)
li r3, 0x2
rlwimi r0, r3, 7, 21, 24
sth r0, 0x10(r1)
b @8008AF74
lhz r0, 0x10(r1)
li r3, 0x3
rlwimi r0, r3, 7, 21, 24
sth r0, 0x10(r1)
b @8008AF74
lhz r0, 0x10(r1)
li r3, 0x4
rlwimi r0, r3, 7, 21, 24
sth r0, 0x10(r1)
b @8008AF74
lhz r0, 0x10(r1)
li r3, 0x5
rlwimi r0, r3, 7, 21, 24
sth r0, 0x10(r1)
b @8008AF74
lhz r0, 0x10(r1)
li r3, 0xf
rlwimi r0, r3, 7, 21, 24
sth r0, 0x10(r1)
b @8008AF74
@8008AF64
lhz r0, 0x10(r1)
li r3, 0x0
rlwimi r0, r3, 7, 21, 24
sth r0, 0x10(r1)
@8008AF74
mr r3, r30
bl fn_80135A70
clrlwi r0, r3, 24
cmplwi r0, 0x9
bgt @8008B000
lis r3, jumptable_802EEBB8@ha
slwi r0, r0, 2
addi r3, r3, jumptable_802EEBB8@l
lwzx r0, r3, r0
mtctr r0
bctr
li r0, 0x1
stb r0, 0x12(r31)
b @8008B008
li r0, 0x2
stb r0, 0x12(r31)
b @8008B008
li r0, 0x5
stb r0, 0x12(r31)
b @8008B008
li r0, 0x3
stb r0, 0x12(r31)
b @8008B008
li r0, 0x4
stb r0, 0x12(r31)
b @8008B008
li r0, 0x7
stb r0, 0x12(r31)
b @8008B008
li r0, 0x2
stb r0, 0x12(r31)
b @8008B008
li r0, 0x2
stb r0, 0x12(r31)
b @8008B008
@8008B000
li r0, 0x0
stb r0, 0x12(r31)
@8008B008
lbz r0, 0x13(r31)
li r3, 0x1
rlwimi r0, r3, 1, 30, 30
mr r3, r28
stb r0, 0x13(r31)
bl fn_8011E8DC
clrlwi r5, r3, 24
lbz r0, 0x13(r31)
neg r4, r5
mr r3, r28
or r4, r4, r5
srwi r4, r4, 31
rlwimi r0, r4, 2, 29, 29
stb r0, 0x13(r31)
bl fn_8011E838
lbz r0, 0x13(r31)
rlwimi r0, r3, 3, 24, 28
mr r3, r28
stb r0, 0x13(r31)
bl fn_8011E850
clrlwi r5, r3, 24
lbz r0, 0x13(r31)
neg r4, r5
mr r3, r30
or r4, r4, r5
srwi r4, r4, 31
rlwimi r0, r4, 0, 31, 31
stb r0, 0x13(r31)
bl fn_80135A70
clrlwi r27, r3, 24
mr r3, r28
bl fn_8011F4D8
mr r4, r3
mr r5, r27
addi r3, r31, 0x8
bl fn_800F9AEC
mr r5, r3
cmpwi r5, 0xa
bge @8008B0C4
add r4, r31, r5
li r0, 0xff
addi r3, r5, 0x9
stb r0, 0x8(r4)
add r3, r31, r3
subfic r5, r5, 0x9
li r4, 0x0
bl memset
@8008B0C4
mr r3, r30
bl fn_80135A70
clrlwi r30, r3, 24
mr r3, r28
bl fn_8011F508
mr r4, r3
mr r5, r30
addi r3, r31, 0x14
bl fn_800F9AEC
mr r5, r3
cmpwi r5, 0x7
bge @8008B114
add r4, r31, r5
li r0, 0xff
addi r3, r5, 0x15
stb r0, 0x14(r4)
add r3, r31, r3
subfic r5, r5, 0x6
li r4, 0x0
bl memset
@8008B114
mr r3, r28
bl fn_8011E820
stb r3, 0x1b(r31)
mr r3, r28
bl fn_8011E7F0
clrlwi r0, r3, 16
mr r3, r28
slwi r4, r0, 8
srawi r0, r0, 8
or r0, r4, r0
clrlwi r0, r0, 16
sth r0, 0x1e(r31)
bl fn_8011F5C8
clrlwi r0, r3, 16
mr r3, r28
slwi r4, r0, 8
srawi r0, r0, 8
or r0, r4, r0
clrlwi r0, r0, 16
sth r0, 0x20(r31)
bl fn_8011F1A0
clrlwi r0, r3, 16
mr r3, r28
slwi r4, r0, 8
srawi r0, r0, 8
or r0, r4, r0
clrlwi r0, r0, 16
sth r0, 0x22(r31)
bl fn_8011F4C0
rlwinm r0, r3, 0, 16, 23
rlwinm r5, r3, 0, 8, 15
slwi r4, r3, 24
srwi r6, r3, 24
slwi r0, r0, 8
srwi r5, r5, 8
or r0, r4, r0
mr r3, r28
or r0, r5, r0
or r0, r6, r0
stw r0, 0x24(r31)
bl fn_8011EE58
clrlwi r0, r3, 24
mr r3, r28
stb r0, 0x29(r31)
bl fn_8011E7D8
sth r3, 0x2a(r31)
li r26, 0x0
mr r27, r31
li r30, 0x0
stb r26, 0x28(r31)
@8008B1DC
mr r3, r28
clrlwi r4, r30, 16
bl fn_8011F228
clrlwi r0, r3, 16
mr r3, r28
slwi r5, r0, 8
clrlwi r4, r30, 16
srawi r0, r0, 8
or r0, r5, r0
clrlwi r0, r0, 16
sth r0, 0x2c(r27)
bl fn_8011F1B8
clrlwi r3, r3, 24
lbz r0, 0x28(r31)
slw r4, r3, r26
mr r3, r28
or r0, r0, r4
clrlwi r4, r30, 16
clrlwi r0, r0, 24
stb r0, 0x28(r31)
bl fn_8011F1F0
addi r0, r30, 0x34
addi r27, r27, 0x2
stbx r3, r31, r0
addi r26, r26, 0x2
addi r30, r30, 0x1
cmpwi r30, 0x4
blt @8008B1DC
mr r3, r28
bl fn_8011F054
clrlwi r0, r3, 24
mr r3, r28
stb r0, 0x38(r31)
bl fn_8011F028
clrlwi r0, r3, 24
mr r3, r28
stb r0, 0x39(r31)
bl fn_8011EFFC
clrlwi r0, r3, 24
mr r3, r28
stb r0, 0x3a(r31)
bl fn_8011EF78
clrlwi r0, r3, 24
mr r3, r28
stb r0, 0x3b(r31)
bl fn_8011EFD0
clrlwi r0, r3, 24
mr r3, r28
stb r0, 0x3c(r31)
bl fn_8011EFA4
clrlwi r0, r3, 24
mr r3, r28
stb r0, 0x3d(r31)
bl fn_8011ECEC
stb r3, 0x3e(r31)
mr r3, r28
bl fn_8011ECC0
stb r3, 0x3f(r31)
mr r3, r28
bl fn_8011EC94
stb r3, 0x40(r31)
mr r3, r28
bl fn_8011EC68
stb r3, 0x41(r31)
mr r3, r28
bl fn_8011EC3C
stb r3, 0x42(r31)
mr r3, r28
bl fn_8011EB48
stb r3, 0x43(r31)
mr r3, r28
bl fn_8011E8F4
stb r3, 0x13(r1)
mr r3, r28
bl fn_8011F580
clrlwi r0, r3, 24
mr r3, r28
stb r0, 0x12(r1)
bl fn_8011F568
lbz r0, 0x11(r1)
rlwimi r0, r3, 0, 25, 31
mr r3, r28
stb r0, 0x11(r1)
bl fn_8011F550
lbz r0, 0x10(r1)
rlwimi r0, r3, 3, 25, 28
mr r3, r28
stb r0, 0x10(r1)
bl fn_8011F538
lbz r0, 0x10(r1)
rlwimi r0, r3, 7, 24, 24
mr r3, r28
stb r0, 0x10(r1)
bl fn_8011EF4C
clrlwi r3, r3, 24
lbz r0, 0xf(r1)
rlwimi r0, r3, 0, 27, 31
mr r3, r28
stb r0, 0xf(r1)
bl fn_8011EF20
lhz r0, 0xe(r1)
rlwimi r0, r3, 5, 22, 26
mr r3, r28
sth r0, 0xe(r1)
bl fn_8011EEF4
clrlwi r3, r3, 24
lbz r0, 0xe(r1)
rlwimi r0, r3, 2, 25, 29
mr r3, r28
stb r0, 0xe(r1)
bl fn_8011EE70
clrlwi r3, r3, 16
lwz r0, 0xc(r1)
rlwimi r0, r3, 15, 12, 16
mr r3, r28
stw r0, 0xc(r1)
bl fn_8011EEC8
lhz r0, 0xc(r1)
rlwimi r0, r3, 4, 23, 27
mr r3, r28
sth r0, 0xc(r1)
bl fn_8011EE9C
clrlwi r3, r3, 24
lbz r0, 0xc(r1)
rlwimi r0, r3, 1, 26, 30
mr r3, r28
stb r0, 0xc(r1)
bl fn_8011E8DC
clrlwi r5, r3, 24
lbz r0, 0xc(r1)
neg r4, r5
mr r3, r28
or r4, r4, r5
srwi r4, r4, 31
rlwimi r0, r4, 6, 25, 25
stb r0, 0xc(r1)
bl fn_8011E868
clrlwi r5, r3, 24
lbz r0, 0xc(r1)
neg r4, r5
mr r3, r28
or r4, r4, r5
srwi r4, r4, 31
rlwimi r0, r4, 7, 24, 24
stb r0, 0xc(r1)
bl fn_8011EC10
lbz r0, 0xb(r1)
rlwimi r0, r3, 0, 29, 31
mr r3, r28
stb r0, 0xb(r1)
bl fn_8011EBE4
lbz r0, 0xb(r1)
rlwimi r0, r3, 3, 26, 28
mr r3, r28
stb r0, 0xb(r1)
bl fn_8011EBB8
clrlwi r3, r3, 24
lhz r0, 0xa(r1)
rlwimi r0, r3, 6, 23, 25
mr r3, r28
sth r0, 0xa(r1)
bl fn_8011EB8C
lbz r0, 0xa(r1)
rlwimi r0, r3, 1, 28, 30
mr r3, r28
stb r0, 0xa(r1)
bl fn_8011EB60
lbz r0, 0xa(r1)
rlwimi r0, r3, 4, 25, 27
mr r3, r28
stb r0, 0xa(r1)
bl fn_8011EB1C
clrlwi r5, r3, 24
lbz r0, 0xa(r1)
neg r4, r5
mr r3, r28
or r4, r4, r5
srwi r4, r4, 31
rlwimi r0, r4, 7, 24, 24
stb r0, 0xa(r1)
bl fn_8011EAF0
clrlwi r5, r3, 24
lbz r0, 0x9(r1)
neg r4, r5
mr r3, r28
or r4, r4, r5
srwi r4, r4, 31
rlwimi r0, r4, 0, 31, 31
stb r0, 0x9(r1)
bl fn_8011EAC4
clrlwi r5, r3, 24
lbz r0, 0x9(r1)
neg r4, r5
mr r3, r28
or r4, r4, r5
srwi r4, r4, 31
rlwimi r0, r4, 1, 30, 30
stb r0, 0x9(r1)
bl fn_8011EA98
clrlwi r5, r3, 24
lbz r0, 0x9(r1)
neg r4, r5
mr r3, r28
or r4, r4, r5
srwi r4, r4, 31
rlwimi r0, r4, 2, 29, 29
stb r0, 0x9(r1)
bl fn_8011EA6C
clrlwi r5, r3, 24
lbz r0, 0x9(r1)
neg r4, r5
mr r3, r28
or r4, r4, r5
srwi r4, r4, 31
rlwimi r0, r4, 3, 28, 28
stb r0, 0x9(r1)
bl fn_8011EA40
clrlwi r5, r3, 24
lbz r0, 0x9(r1)
neg r4, r5
mr r3, r28
or r4, r4, r5
srwi r4, r4, 31
rlwimi r0, r4, 4, 27, 27
stb r0, 0x9(r1)
bl fn_8011EA14
clrlwi r5, r3, 24
lbz r0, 0x9(r1)
neg r4, r5
mr r3, r28
or r4, r4, r5
srwi r4, r4, 31
rlwimi r0, r4, 5, 26, 26
stb r0, 0x9(r1)
bl fn_8011E9E8
clrlwi r5, r3, 24
lbz r0, 0x9(r1)
neg r4, r5
mr r3, r28
or r4, r4, r5
srwi r4, r4, 31
rlwimi r0, r4, 6, 25, 25
stb r0, 0x9(r1)
bl fn_8011E9BC
clrlwi r5, r3, 24
lbz r0, 0x9(r1)
neg r4, r5
mr r3, r28
or r4, r4, r5
srwi r4, r4, 31
rlwimi r0, r4, 7, 24, 24
stb r0, 0x9(r1)
bl fn_8011E990
clrlwi r5, r3, 24
lbz r0, 0x8(r1)
neg r4, r5
mr r3, r28
or r4, r4, r5
srwi r4, r4, 31
rlwimi r0, r4, 0, 31, 31
stb r0, 0x8(r1)
bl fn_8011E964
clrlwi r5, r3, 24
lbz r0, 0x8(r1)
neg r4, r5
mr r3, r28
or r4, r4, r5
srwi r4, r4, 31
rlwimi r0, r4, 1, 30, 30
stb r0, 0x8(r1)
bl fn_8011E938
clrlwi r5, r3, 24
lbz r0, 0x8(r1)
neg r4, r5
mr r3, r28
or r4, r4, r5
srwi r4, r4, 31
rlwimi r0, r4, 2, 29, 29
stb r0, 0x8(r1)
bl fn_8011E90C
lbz r0, 0x8(r1)
rlwimi r0, r3, 3, 25, 28
mr r3, r28
stb r0, 0x8(r1)
bl fn_8011E7A4
lbz r0, 0x8(r1)
rlwimi r0, r3, 7, 24, 24
mr r3, r28
li r27, 0x0
stb r0, 0x8(r1)
li r4, 0x4
bl fn_80121ADC
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8008B698
mr r3, r28
li r4, 0x4
bl fn_80121984
extsh r0, r3
slwi r0, r0, 8
ori r0, r0, 0x80
clrlwi r27, r0, 16
b @8008B754
@8008B698
mr r3, r28
li r4, 0x5
bl fn_80121ADC
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8008B6BC
ori r0, r27, 0x40
clrlwi r27, r0, 16
b @8008B754
@8008B6BC
mr r3, r28
li r4, 0x7
bl fn_80121ADC
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8008B6E0
ori r0, r27, 0x20
clrlwi r27, r0, 16
b @8008B754
@8008B6E0
mr r3, r28
li r4, 0x6
bl fn_80121ADC
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8008B704
ori r0, r27, 0x10
clrlwi r27, r0, 16
b @8008B754
@8008B704
mr r3, r28
li r4, 0x3
bl fn_80121ADC
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8008B728
ori r0, r27, 0x8
clrlwi r27, r0, 16
b @8008B754
@8008B728
mr r3, r28
li r4, 0x8
bl fn_80121ADC
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8008B754
mr r3, r28
li r4, 0x8
bl fn_8012189C
extsb r0, r3
clrlwi r27, r0, 16
@8008B754
mr r3, r28
clrlwi r27, r27, 16
bl fn_8011F45C
rlwinm r0, r27, 0, 16, 23
rlwinm r5, r27, 0, 8, 15
slwi r4, r27, 24
clrrwi r6, r3, 12
slwi r0, r0, 8
srwi r3, r5, 8
or r0, r4, r0
srwi r4, r27, 24
or r0, r3, r0
mr r3, r28
or r0, r4, r0
or r0, r0, r6
stw r0, 0x50(r31)
bl fn_8011F4A8
stb r3, 0x54(r31)
mr r3, r28
bl fn_8011E808
stb r3, 0x55(r31)
mr r3, r28
bl fn_8011F15C
clrlwi r0, r3, 16
mr r3, r28
slwi r4, r0, 8
srawi r0, r0, 8
or r0, r4, r0
clrlwi r0, r0, 16
sth r0, 0x58(r31)
bl fn_8011F188
clrlwi r0, r3, 16
mr r3, r28
slwi r4, r0, 8
srawi r0, r0, 8
or r0, r4, r0
clrlwi r0, r0, 16
sth r0, 0x56(r31)
bl fn_8011F130
clrlwi r0, r3, 16
mr r3, r28
slwi r4, r0, 8
srawi r0, r0, 8
or r0, r4, r0
clrlwi r0, r0, 16
sth r0, 0x5a(r31)
bl fn_8011F104
clrlwi r0, r3, 16
mr r3, r28
slwi r4, r0, 8
srawi r0, r0, 8
or r0, r4, r0
clrlwi r0, r0, 16
sth r0, 0x5c(r31)
bl fn_8011F080
clrlwi r0, r3, 16
mr r3, r28
slwi r4, r0, 8
srawi r0, r0, 8
or r0, r4, r0
clrlwi r0, r0, 16
sth r0, 0x5e(r31)
bl fn_8011F0D8
clrlwi r0, r3, 16
mr r3, r28
slwi r4, r0, 8
srawi r0, r0, 8
or r0, r4, r0
clrlwi r0, r0, 16
sth r0, 0x60(r31)
bl fn_8011F0AC
lwz r11, 0x10(r1)
clrlwi r0, r3, 16
lwz r12, 0xc(r1)
addi r4, r31, 0x20
rlwinm r7, r11, 0, 16, 23
slwi r5, r0, 8
srawi r3, r0, 8
rlwinm r10, r11, 0, 8, 15
lwz r0, 0x8(r1)
or r3, r5, r3
clrlwi r27, r3, 16
rlwinm r5, r12, 0, 16, 23
rlwinm r8, r12, 0, 8, 15
rlwinm r3, r0, 0, 16, 23
rlwinm r6, r0, 0, 8, 15
slwi r9, r11, 24
slwi r7, r7, 8
srwi r10, r10, 8
or r9, r9, r7
sth r27, 0x62(r31)
srwi r11, r11, 24
slwi r7, r12, 24
or r9, r10, r9
slwi r5, r5, 8
or r9, r11, r9
srwi r8, r8, 8
or r7, r7, r5
stw r9, 0x44(r31)
srwi r9, r12, 24
slwi r5, r0, 24
or r7, r8, r7
slwi r3, r3, 8
or r7, r9, r7
srwi r6, r6, 8
or r5, r5, r3
stw r7, 0x48(r31)
srwi r7, r0, 24
mr r3, r4
or r0, r6, r5
or r0, r7, r0
stw r0, 0x4c(r31)
li r0, 0x3
mtctr r0
@8008B91C
lhz r5, 0x0(r3)
lhz r7, 0x2(r3)
srawi r0, r5, 8
slwi r5, r5, 8
lhz r8, 0x4(r3)
or r6, r5, r0
srawi r0, r7, 8
slwi r5, r7, 8
lhz r7, 0x6(r3)
clrlwi r11, r6, 16
or r10, r5, r0
srawi r0, r8, 8
slwi r5, r8, 8
lhz r8, 0x8(r3)
or r6, r5, r0
srawi r0, r7, 8
slwi r5, r7, 8
lhz r7, 0xa(r3)
or r9, r5, r0
srawi r0, r8, 8
slwi r5, r8, 8
lhz r26, 0xc(r3)
lhz r12, 0xe(r3)
or r8, r5, r0
srawi r0, r7, 8
slwi r5, r7, 8
or r7, r5, r0
srawi r0, r26, 8
slwi r5, r26, 8
add r29, r29, r11
clrlwi r11, r10, 16
clrlwi r10, r6, 16
add r29, r29, r11
or r6, r5, r0
add r29, r29, r10
clrlwi r0, r9, 16
add r29, r29, r0
clrlwi r0, r8, 16
add r29, r29, r0
clrlwi r7, r7, 16
slwi r5, r12, 8
srawi r0, r12, 8
or r0, r5, r0
add r29, r29, r7
clrlwi r5, r6, 16
addi r3, r3, 0x10
add r29, r29, r5
clrlwi r0, r0, 16
add r29, r29, r0
bdnz @8008B91C
clrlwi r0, r29, 16
slwi r3, r0, 8
srawi r0, r0, 8
or r0, r3, r0
clrlwi r0, r0, 16
sth r0, 0x1c(r31)
li r0, 0x2
mtctr r0
@8008BA04
lwz r3, 0x0(r31)
lwz r0, 0x4(r31)
lwz r5, 0x0(r4)
xor r0, r3, r0
xor r0, r5, r0
stw r0, 0x0(r4)
lwz r3, 0x0(r31)
lwz r0, 0x4(r31)
lwz r5, 0x4(r4)
xor r0, r3, r0
xor r0, r5, r0
stw r0, 0x4(r4)
lwz r3, 0x0(r31)
lwz r0, 0x4(r31)
lwz r5, 0x8(r4)
xor r0, r3, r0
xor r0, r5, r0
stw r0, 0x8(r4)
lwz r3, 0x0(r31)
lwz r0, 0x4(r31)
lwz r5, 0xc(r4)
xor r0, r3, r0
xor r0, r5, r0
stw r0, 0xc(r4)
lwz r3, 0x0(r31)
lwz r0, 0x4(r31)
lwz r5, 0x10(r4)
xor r0, r3, r0
xor r0, r5, r0
stw r0, 0x10(r4)
lwz r3, 0x0(r31)
lwz r0, 0x4(r31)
lwz r5, 0x14(r4)
xor r0, r3, r0
xor r0, r5, r0
stw r0, 0x14(r4)
addi r4, r4, 0x18
bdnz @8008BA04
lwz r6, 0x0(r31)
lis r3, 0xaaab
subi r3, r3, 0x5555
rlwinm r0, r6, 0, 16, 23
rlwinm r5, r6, 0, 8, 15
slwi r4, r6, 24
srwi r6, r6, 24
slwi r0, r0, 8
srwi r5, r5, 8
or r0, r4, r0
or r0, r5, r0
or r27, r6, r0
mulhwu r0, r3, r27
srwi r0, r0, 4
mulli r0, r0, 0x18
subf r27, r0, r27
mulhwu r0, r3, r27
srwi r0, r0, 2
cmplwi r0, 0x0
beq @8008BB24
mulli r26, r0, 0xc
addi r3, r1, 0x14
li r5, 0xc
add r4, r31, r26
addi r4, r4, 0x20
bl memcpy
mr r5, r26
addi r3, r31, 0x2c
addi r4, r31, 0x20
bl fn_800C8174
addi r3, r31, 0x20
addi r4, r1, 0x14
li r5, 0xc
bl memcpy
@8008BB24
lis r3, 0xaaab
subi r0, r3, 0x5555
mulhwu r0, r0, r27
srwi r0, r0, 2
mulli r0, r0, 0x6
subf r27, r0, r27
srwi r0, r27, 1
cmplwi r0, 0x0
beq @8008BB84
mulli r26, r0, 0xc
addi r3, r1, 0x14
li r5, 0xc
add r4, r31, r26
addi r4, r4, 0x2c
bl memcpy
addi r28, r31, 0x2c
mr r5, r26
mr r4, r28
addi r3, r31, 0x38
bl fn_800C8174
mr r3, r28
addi r4, r1, 0x14
li r5, 0xc
bl memcpy
@8008BB84
clrlwi r0, r27, 31
cmplwi r0, 0x0
beq @8008BBC8
addi r26, r31, 0x44
addi r3, r1, 0x14
mr r4, r26
li r5, 0xc
bl memcpy
addi r27, r31, 0x38
mr r3, r26
mr r4, r27
li r5, 0xc
bl memcpy
mr r3, r27
addi r4, r1, 0x14
li r5, 0xc
bl memcpy
@8008BBC8
lmw r26, 0x28(r1)
lwz r0, 0x44(r1)
mtlr r0
addi r1, r1, 0x40
blr
