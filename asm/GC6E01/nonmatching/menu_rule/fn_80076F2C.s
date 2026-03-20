stwu r1, -0x40(r1)
mflr r0
stw r0, 0x44(r1)
stmw r20, 0x10(r1)
mr r22, r3
mr r23, r4
mr r24, r5
li r31, 0x0
li r30, 0x1
li r29, 0x1
li r28, 0x0
@80076F58
mr r3, r22
clrlwi r4, r28, 16
bl fn_8012AC08
mr r26, r3
li r21, 0x0
cntlzw r0, r26
srwi r20, r0, 5
cmpwi r20, 0x0
bne @80076F94
li r4, 0x0
li r5, 0x6e
li r6, 0x0
bl fn_8012640C
cmpwi r3, 0x0
bne @80076F98
@80076F94
li r21, 0x1
@80076F98
cmpwi r21, 0x0
bne @80077178
cmpwi r20, 0x0
li r20, 0x0
bne @80076FC8
mr r3, r26
li r4, 0x0
li r5, 0x6e
li r6, 0x0
bl fn_8012640C
cmpwi r3, 0x0
bne @80076FCC
@80076FC8
li r20, 0x1
@80076FCC
cmpwi r20, 0x0
beq @80076FDC
li r0, 0x0
b @80077010
@80076FDC
mr r3, r26
li r20, 0x0
bl fn_8011E8DC
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80077008
mr r3, r26
bl fn_80123FBC
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8007700C
@80077008
li r20, 0x1
@8007700C
clrlwi r0, r20, 24
@80077010
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @80077178
mr r3, r26
bl fn_8011F4A8
clrlwi r0, r3, 24
addi r27, r28, 0x1
add r31, r31, r0
b @80077170
@80077034
mr r3, r22
clrlwi r4, r27, 16
bl fn_8012AC08
mr r25, r3
li r20, 0x0
cntlzw r0, r25
srwi r21, r0, 5
cmpwi r21, 0x0
bne @80077070
li r4, 0x0
li r5, 0x6e
li r6, 0x0
bl fn_8012640C
cmpwi r3, 0x0
bne @80077074
@80077070
li r20, 0x1
@80077074
cmpwi r20, 0x0
bne @8007716C
cmpwi r21, 0x0
li r21, 0x0
bne @800770A4
mr r3, r25
li r4, 0x0
li r5, 0x6e
li r6, 0x0
bl fn_8012640C
cmpwi r3, 0x0
bne @800770A8
@800770A4
li r21, 0x1
@800770A8
cmpwi r21, 0x0
beq @800770B8
li r0, 0x0
b @800770EC
@800770B8
mr r3, r25
li r21, 0x0
bl fn_8011E8DC
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @800770E4
mr r3, r25
bl fn_80123FBC
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @800770E8
@800770E4
li r21, 0x1
@800770E8
clrlwi r0, r21, 24
@800770EC
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @8007716C
mr r3, r26
bl fn_8011F1A0
clrlwi r0, r3, 16
cmplwi r0, 0x0
beq @8007713C
mr r3, r25
bl fn_8011F1A0
clrlwi r21, r3, 16
mr r3, r26
bl fn_8011F1A0
clrlwi r0, r3, 16
subf r3, r0, r21
subf r0, r21, r0
or r0, r3, r0
srwi r0, r0, 31
and r0, r29, r0
clrlwi r29, r0, 24
@8007713C
mr r3, r25
bl fn_8011F5C8
clrlwi r25, r3, 16
mr r3, r26
bl fn_8011F5C8
clrlwi r0, r3, 16
subf r3, r0, r25
subf r0, r25, r0
or r0, r3, r0
srwi r0, r0, 31
and r0, r30, r0
clrlwi r30, r0, 24
@8007716C
addi r27, r27, 0x1
@80077170
cmplwi r27, 0x6
blt @80077034
@80077178
addi r28, r28, 0x1
cmplwi r28, 0x6
blt @80076F58
cmpwi r24, 0x2
beq @800771F0
bge @800771A0
cmpwi r24, 0x0
beq @800771AC
bge @800771C8
b @80077294
@800771A0
cmpwi r24, 0x4
bge @80077294
b @80077218
@800771AC
lha r0, 0x4(r23)
srwi r3, r31, 31
srawi r4, r0, 31
subfc r0, r31, r0
adde r0, r4, r3
clrlwi r3, r0, 24
b @80077298
@800771C8
lbz r0, 0xc(r23)
li r3, 0x0
cmplwi r0, 0x0
bne @800771E4
clrlwi r0, r30, 24
cmplwi r0, 0x0
beq @800771E8
@800771E4
li r3, 0x1
@800771E8
clrlwi r3, r3, 24
b @80077298
@800771F0
lbz r0, 0xd(r23)
li r3, 0x0
cmplwi r0, 0x0
bne @8007720C
clrlwi r0, r29, 24
cmplwi r0, 0x0
beq @80077210
@8007720C
li r3, 0x1
@80077210
clrlwi r3, r3, 24
b @80077298
@80077218
li r20, 0x0
li r21, 0x0
b @80077268
@80077224
mr r3, r22
mr r4, r21
bl fn_8012AC08
cmplwi r3, 0x0
li r24, 0x0
beq @80077254
li r4, 0x0
li r5, 0x6e
li r6, 0x0
bl fn_8012640C
cmpwi r3, 0x0
bne @80077258
@80077254
li r24, 0x1
@80077258
cmpwi r24, 0x0
bne @80077264
addi r20, r20, 0x1
@80077264
addi r21, r21, 0x1
@80077268
clrlwi r0, r21, 16
cmplwi r0, 0x6
blt @80077224
clrlwi r0, r20, 16
lha r5, 0x6(r23)
srawi r4, r0, 31
srwi r3, r5, 31
subfc r0, r5, r0
adde r0, r4, r3
clrlwi r3, r0, 24
b @80077298
@80077294
li r3, 0x0
@80077298
lmw r20, 0x10(r1)
lwz r0, 0x44(r1)
mtlr r0
addi r1, r1, 0x40
blr
