stwu r1, -0x30(r1)
mflr r0
stw r0, 0x34(r1)
stmw r23, 0xc(r1)
mr r25, r3
mr r26, r4
li r28, 0x0
li r24, 0x0
@800767D8
mr r3, r25
mr r4, r26
mr r5, r24
bl fn_80076F2C
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @800767FC
li r0, 0x0
b @8007680C
@800767FC
addi r24, r24, 0x1
cmpwi r24, 0x4
blt @800767D8
li r0, 0x1
@8007680C
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @80076820
li r3, 0x0
b @80076A78
@80076820
li r29, 0x0
@80076824
mr r3, r25
clrlwi r4, r29, 16
bl fn_8012AC08
mr r27, r3
cmplwi r27, 0x0
beq @80076A60
bl fn_80123FBC
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80076A60
cntlzw r0, r27
li r30, 0x0
srwi r31, r0, 5
@80076858
cmpwi r31, 0x0
li r24, 0x0
bne @80076880
mr r3, r27
li r4, 0x0
li r5, 0x6e
li r6, 0x0
bl fn_8012640C
cmpwi r3, 0x0
bne @80076884
@80076880
li r24, 0x1
@80076884
cmpwi r24, 0x0
beq @80076894
li r0, 0x1
b @80076A24
@80076894
cmpwi r30, 0x1
beq @800768E0
bge @800768AC
cmpwi r30, 0x0
bge @800768B8
b @80076A08
@800768AC
cmpwi r30, 0x3
bge @80076A08
b @80076908
@800768B8
mr r3, r27
bl fn_8011F4A8
clrlwi r5, r3, 24
lha r0, 0x0(r26)
srawi r4, r5, 31
srwi r3, r0, 31
subfc r0, r0, r5
adde r0, r4, r3
clrlwi r0, r0, 24
b @80076A24
@800768E0
mr r3, r27
bl fn_8011F4A8
lha r0, 0x2(r26)
clrlwi r5, r3, 24
srwi r3, r5, 31
srawi r4, r0, 31
subfc r0, r5, r0
adde r0, r4, r3
clrlwi r0, r0, 24
b @80076A24
@80076908
mr r3, r27
bl fn_8011F1A0
mr r23, r3
bl fn_8006B420
clrlwi r0, r23, 16
mr r24, r3
cmpwi r0, 0xaf
beq @80076940
bge @80076948
cmpwi r0, 0x0
beq @80076938
b @80076948
@80076938
li r3, 0x1
b @80076950
@80076940
li r3, 0x0
b @80076950
@80076948
mr r3, r23
bl fn_80142984
@80076950
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80076964
li r0, 0x0
b @80076A24
@80076964
lwz r0, 0x8(r24)
cmpwi r0, 0x1
beq @80076994
bge @80076980
cmpwi r0, 0x0
bge @8007698C
b @80076A00
@80076980
cmpwi r0, 0x3
bge @80076A00
b @800769A8
@8007698C
li r0, 0x1
b @80076A24
@80076994
clrlwi r0, r23, 16
cntlzw r0, r0
srwi r0, r0, 5
clrlwi r0, r0, 24
b @80076A24
@800769A8
lis r3, lbl_802EE458@ha
lwz r0, lbl_80478928@sda21(r0)
addi r5, r3, lbl_802EE458@l
li r4, 0x0
clrlwi r3, r23, 16
mtctr r0
cmplwi r0, 0x0
ble @800769F8
@800769C8
lhz r0, 0x0(r5)
cmplw r3, r0
bne @800769EC
addi r0, r4, 0x18
lbzx r0, r24, r0
cntlzw r0, r0
srwi r0, r0, 5
clrlwi r0, r0, 24
b @80076A24
@800769EC
addi r5, r5, 0x2
addi r4, r4, 0x1
bdnz @800769C8
@800769F8
li r0, 0x1
b @80076A24
@80076A00
li r0, 0x0
b @80076A24
@80076A08
lis r3, lbl_80268A48@ha
lis r5, lbl_80268A58@ha
addi r3, r3, lbl_80268A48@l
li r4, 0xfb
addi r5, r5, lbl_80268A58@l
bl fn_80196E10
li r0, 0x0
@80076A24
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @80076A38
li r0, 0x0
b @80076A48
@80076A38
addi r30, r30, 0x1
cmpwi r30, 0x3
blt @80076858
li r0, 0x1
@80076A48
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @80076A5C
li r3, 0x0
b @80076A78
@80076A5C
addi r28, r28, 0x1
@80076A60
addi r29, r29, 0x1
cmpwi r29, 0x6
blt @80076824
neg r0, r28
andc r0, r0, r28
srwi r3, r0, 31
@80076A78
lmw r23, 0xc(r1)
lwz r0, 0x34(r1)
mtlr r0
addi r1, r1, 0x30
blr
