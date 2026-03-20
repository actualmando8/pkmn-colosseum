stwu r1, -0x50(r1)
mflr r0
stw r0, 0x54(r1)
stmw r16, 0x10(r1)
mr r31, r3
li r16, 0x0
bl fn_801EF634
mr r18, r3
li r27, -0x1
li r26, 0x1
bl fn_8025D9A8
mr r29, r3
bl fn_8025DBB0
mr r30, r3
li r25, 0x0
li r24, 0x0
li r23, 0x0
li r22, 0x1
li r20, 0x1
li r21, 0x0
li r3, 0x0
li r4, 0x2
bl fn_80129280
mr r17, r3
bl fn_8012A80C
mr r0, r3
mr r3, r17
mr r17, r0
bl fn_8012A7C4
lis r4, 0x6666
addi r5, r30, 0x1
addi r0, r4, 0x6667
mr r19, r3
mulhw r0, r0, r5
clrlwi r28, r18, 16
srawi r0, r0, 2
srwi r3, r0, 31
add r0, r0, r3
mulli r0, r0, 0xa
subf r18, r0, r5
@80063100
cmplwi r16, 0xc
bgt @80063788
lis r3, jumptable_802ED9B8@ha
slwi r0, r16, 2
addi r3, r3, jumptable_802ED9B8@l
lwzx r0, r3, r0
mtctr r0
bctr
li r3, 0xdf
li r4, 0x0
bl fn_8010264C
li r3, 0xba
li r4, 0x1
bl fn_8010264C
cmpwi r28, 0x5
beq @80063160
bge @80063154
cmpwi r28, 0x2
beq @80063160
bge @80063260
b @80063290
@80063154
cmpwi r28, 0x8
bge @80063290
b @80063260
@80063160
cmpwi r29, 0x1
beq @80063194
bge @800631C0
cmpwi r29, 0x0
bge @80063178
b @800631C0
@80063178
cmpwi r30, 0x7
bne @800631C0
bl fn_8025D164
bl fn_8006ADB4
li r25, 0x1
bl fn_800637B0
b @800631C0
@80063194
cmpwi r18, 0x0
bne @800631B0
bl fn_8025D164
mr r16, r3
bl fn_8006ADEC
add r3, r16, r3
bl fn_8006ADB4
@800631B0
addi r0, r30, 0x1
cmpwi r0, 0x64
bne @800631C0
li r25, 0x1
@800631C0
clrlwi r0, r25, 24
cmplwi r0, 0x0
beq @800631F4
cmpwi r29, 0x1
bne @800631EC
li r3, 0xafd
bl fn_801906A0
cmplwi r3, 0x0
bne @800631EC
bl fn_801EE398
mr r21, r3
@800631EC
li r16, 0x5
b @80063788
@800631F4
li r3, 0x0
bl fn_80062284
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80063244
li r3, 0x2
li r4, 0x3c10
li r5, 0x1
li r6, 0x1
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
bl fn_8025DB2C
li r3, 0x2
li r4, 0x30dd
li r5, 0x0
li r6, 0x1
bl fn_80106D3C
li r16, 0x1
b @80063788
@80063244
li r3, 0x2
li r4, 0x30dd
li r5, 0x0
li r6, 0x1
bl fn_80106D3C
li r16, 0x1
b @80063788
@80063260
bl fn_8025DB5C
cmpwi r3, 0x0
bne @80063288
cmpwi r29, 0x1
bne @8006327C
li r27, 0x105
b @80063280
@8006327C
li r27, 0xac
@80063280
li r16, 0x9
b @80063788
@80063288
li r16, 0x2
b @80063788
@80063290
li r3, 0x2
li r4, 0x3da4
li r5, 0x0
li r6, 0x1
bl fn_80106D3C
li r16, 0x9
b @80063788
bl fn_801046B8
mr r4, r3
li r3, 0xec
li r5, 0x0
li r6, 0x8
li r7, 0x0
li r8, 0x0
crclr 6
bl fn_801026A4
lha r4, lbl_80478920@sda21(r0)
li r3, 0xec
lha r5, lbl_80478922@sda21(r0)
bl fn_80102868
li r3, 0xec
li r4, 0x1
bl fn_801045A8
li r3, 0xec
bl fn_801043A4
extsb r16, r3
li r3, 0xec
li r4, 0x0
li r5, 0x1
bl fn_80102568
li r3, 0x1
bl fn_801069FC
cmpwi r16, 0x0
bne @8006333C
clrlwi r0, r22, 24
li r24, 0x1
cmplwi r0, 0x0
beq @8006332C
bl fn_8025DB80
@8006332C
li r27, 0xd1
li r23, 0x0
li r16, 0x9
b @80063788
@8006333C
clrlwi r0, r22, 24
li r24, 0x1
cmplwi r0, 0x0
beq @80063350
bl fn_8025DB80
@80063350
li r16, 0x9
li r23, 0x1
li r27, 0xac
b @80063788
li r3, 0x2
li r4, 0x44e3
li r5, 0x0
li r6, 0x1
bl fn_80106D3C
lha r4, lbl_80478920@sda21(r0)
li r3, 0x0
lha r5, lbl_80478922@sda21(r0)
li r6, 0x1
bl fn_8001E074
extsb r16, r3
li r3, 0x1
bl fn_801069FC
cmpwi r16, 0x0
bne @800633A4
li r16, 0xc
b @80063788
@800633A4
li r16, 0x1
b @80063788
bl fn_8025DB5C
mr r4, r3
li r3, 0x30
bl fn_80132A38
li r3, 0x2
li r4, 0x3c13
li r5, 0x0
li r6, 0x1
bl fn_80106D3C
lha r4, lbl_80478920@sda21(r0)
li r3, 0x0
lha r5, lbl_80478922@sda21(r0)
li r6, 0x0
bl fn_8001E074
extsb r16, r3
li r3, 0x1
bl fn_801069FC
cmpwi r16, 0x0
bne @80063408
bl fn_8025DAF4
li r16, 0x9
li r27, 0xd1
b @80063788
@80063408
li r16, 0x3
b @80063788
bl fn_8025DB5C
mr r4, r3
li r3, 0x30
bl fn_80132A38
li r3, 0x2
li r4, 0x44df
li r5, 0x0
li r6, 0x1
bl fn_80106D3C
lha r4, lbl_80478920@sda21(r0)
li r3, 0x0
lha r5, lbl_80478922@sda21(r0)
li r6, 0x1
bl fn_8001E074
extsb r16, r3
li r3, 0x1
bl fn_801069FC
cmpwi r16, 0x0
bne @80063468
li r16, 0x9
li r27, 0xac
b @80063788
@80063468
li r16, 0x2
b @80063788
bl fn_8006ADEC
mr r0, r3
li r3, 0x30
mr r4, r0
bl fn_80132A38
li r3, 0x3cc
li r4, 0x0
li r5, 0x0
bl fn_80166AB8
li r3, 0x2
li r4, 0x3c11
li r5, 0x1
li r6, 0x1
bl fn_80106D3C
li r3, 0x0
bl fn_8006B09C
bl fn_8006A7D0
bl fn_8006AC6C
clrlwi r0, r21, 24
clrlwi r3, r3, 16
cmplwi r0, 0x0
beq @800634D0
li r16, 0x8
b @80063788
@800634D0
cmpwi r3, 0x0
beq @800634E8
blt @800634FC
cmpwi r3, 0x3
bge @800634FC
b @800634F0
@800634E8
li r16, 0x6
b @80063788
@800634F0
li r27, 0x105
li r16, 0xc
b @80063788
@800634FC
li r16, 0x6
b @80063788
li r3, 0x2
li r4, 0x3c23
li r5, 0x0
li r6, 0x1
bl fn_80106D3C
lha r4, lbl_80478920@sda21(r0)
li r3, 0x0
lha r5, lbl_80478922@sda21(r0)
li r6, 0x0
bl fn_8001E074
extsb r0, r3
cmpwi r0, 0x0
bne @80063544
li r27, 0x105
li r16, 0xc
b @80063788
@80063544
li r16, 0xb
b @80063788
li r3, 0x2
li r4, 0x3c0f
li r5, 0x0
li r6, 0x1
bl fn_80106D3C
lha r4, lbl_80478920@sda21(r0)
li r3, 0x0
lha r5, lbl_80478922@sda21(r0)
li r6, 0x1
bl fn_8001E074
extsb r0, r3
cmpwi r0, 0x0
bne @8006358C
li r27, 0xac
li r16, 0xc
b @80063788
@8006358C
li r16, 0xa
b @80063788
li r3, 0x2
li r4, 0x3c03
li r5, 0x0
li r6, 0x1
bl fn_80106D3C
lha r4, lbl_80478920@sda21(r0)
li r3, 0x0
lha r5, lbl_80478922@sda21(r0)
li r6, 0x0
bl fn_8001E074
extsb r16, r3
li r3, 0x1
bl fn_801069FC
cmpwi r16, 0x0
bne @800635F0
clrlwi r0, r20, 24
li r27, 0xac
cmplwi r0, 0x0
li r16, 0x9
beq @800635E8
bl fn_8025D06C
@800635E8
li r23, 0x1
b @80063788
@800635F0
li r16, 0x7
b @80063788
li r3, 0x2
li r4, 0x3c41
li r5, 0x0
li r6, 0x1
bl fn_80106D3C
lha r4, lbl_80478920@sda21(r0)
li r3, 0x0
lha r5, lbl_80478922@sda21(r0)
li r6, 0x1
bl fn_8001E074
extsb r0, r3
cmpwi r0, 0x0
bne @80063658
li r3, 0x0
li r4, 0x2
bl fn_80129280
mr r4, r17
mr r16, r3
bl fn_8012A824
mr r3, r16
mr r4, r19
bl fn_8012A7DC
li r16, 0xc
b @80063788
@80063658
li r16, 0x6
b @80063788
li r3, 0x2
li r4, 0x3c12
li r5, 0x0
li r6, 0x1
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
li r3, 0xdf
li r4, 0x1c6
bl fn_801080CC
li r3, 0xba
li r4, 0x1c6
bl fn_801080CC
b @8006369C
@80063698
bl fn_800F0308
@8006369C
li r3, 0xdf
bl fn_801070F4
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80063698
b @800636B8
@800636B4
bl fn_800F0308
@800636B8
li r3, 0xba
bl fn_801070F4
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @800636B4
bl fn_80069944
bl fn_80062834
li r3, 0x1
bl fn_80061028
li r3, 0xdf
li r4, 0x0
li r5, 0x1
bl fn_80102568
bl fn_800886D0
li r27, 0x105
li r16, 0x9
b @80063788
clrlwi r0, r23, 24
cmplwi r0, 0x0
beq @80063768
clrlwi r0, r24, 24
cmplwi r0, 0x0
beq @80063740
li r3, 0x0
bl fn_800889E4
cmpwi r3, 0x0
bge @80063730
li r16, 0x4
li r22, 0x0
b @80063788
@80063730
mr r3, r31
li r26, 0x0
bl fn_80069C0C
b @80063788
@80063740
bl fn_80088D84
cmpwi r3, 0x0
bge @80063758
li r16, 0x6
li r20, 0x0
b @80063788
@80063758
mr r3, r31
li r26, 0x0
bl fn_80069C0C
b @80063788
@80063768
clrlwi r0, r24, 24
li r26, 0x0
cmplwi r0, 0x0
beq @80063788
mr r3, r31
bl fn_80069C0C
b @80063788
li r26, 0x0
@80063788
cmpwi r26, 0x0
bne @80063100
li r3, 0x1
bl fn_801069FC
mr r3, r27
lmw r16, 0x10(r1)
lwz r0, 0x54(r1)
mtlr r0
addi r1, r1, 0x50
blr
