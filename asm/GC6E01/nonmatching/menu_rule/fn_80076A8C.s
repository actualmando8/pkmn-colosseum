stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stmw r26, 0x8(r1)
mr r30, r3
mr r31, r4
cmpwi r6, 0x2
beq @80076CBC
bge @80076AC0
cmpwi r6, 0x0
beq @80076ACC
bge @80076ADC
b @80076F14
@80076AC0
cmpwi r6, 0x4
bge @80076F14
b @80076EB8
@80076ACC
mr r4, r5
mr r5, r6
bl fn_80076F2C
b @80076F18
@80076ADC
lbz r0, 0xc(r5)
cmplwi r0, 0x0
beq @80076AF0
li r3, 0x1
b @80076F18
@80076AF0
cntlzw r0, r31
li r29, 0x0
srwi r28, r0, 5
cmpwi r28, 0x0
bne @80076B20
mr r3, r31
li r4, 0x0
li r5, 0x6e
li r6, 0x0
bl fn_8012640C
cmpwi r3, 0x0
bne @80076B24
@80076B20
li r29, 0x1
@80076B24
cmpwi r29, 0x0
bne @80076BA8
cmpwi r28, 0x0
li r28, 0x0
bne @80076B54
mr r3, r31
li r4, 0x0
li r5, 0x6e
li r6, 0x0
bl fn_8012640C
cmpwi r3, 0x0
bne @80076B58
@80076B54
li r28, 0x1
@80076B58
cmpwi r28, 0x0
beq @80076B68
li r0, 0x0
b @80076B9C
@80076B68
mr r3, r31
li r28, 0x0
bl fn_8011E8DC
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80076B94
mr r3, r31
bl fn_80123FBC
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80076B98
@80076B94
li r28, 0x1
@80076B98
clrlwi r0, r28, 24
@80076B9C
clrlwi r0, r0, 24
cmplwi r0, 0x0
beq @80076BB0
@80076BA8
li r3, 0x1
b @80076F18
@80076BB0
li r27, 0x0
@80076BB4
mr r3, r30
clrlwi r4, r27, 16
bl fn_8012AC08
mr r26, r3
cmplw r26, r31
beq @80076CA8
cntlzw r0, r26
li r29, 0x0
srwi r28, r0, 5
cmpwi r28, 0x0
bne @80076BF8
li r4, 0x0
li r5, 0x6e
li r6, 0x0
bl fn_8012640C
cmpwi r3, 0x0
bne @80076BFC
@80076BF8
li r29, 0x1
@80076BFC
cmpwi r29, 0x0
bne @80076CA8
cmpwi r28, 0x0
li r28, 0x0
bne @80076C2C
mr r3, r26
li r4, 0x0
li r5, 0x6e
li r6, 0x0
bl fn_8012640C
cmpwi r3, 0x0
bne @80076C30
@80076C2C
li r28, 0x1
@80076C30
cmpwi r28, 0x0
beq @80076C40
li r0, 0x0
b @80076C74
@80076C40
mr r3, r26
li r28, 0x0
bl fn_8011E8DC
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80076C6C
mr r3, r26
bl fn_80123FBC
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80076C70
@80076C6C
li r28, 0x1
@80076C70
clrlwi r0, r28, 24
@80076C74
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @80076CA8
mr r3, r31
bl fn_8011F5C8
clrlwi r28, r3, 16
mr r3, r26
bl fn_8011F5C8
clrlwi r0, r3, 16
cmplw r0, r28
bne @80076CA8
li r3, 0x0
b @80076F18
@80076CA8
addi r27, r27, 0x1
cmpwi r27, 0x6
blt @80076BB4
li r3, 0x1
b @80076F18
@80076CBC
lbz r0, 0xd(r5)
cmplwi r0, 0x0
beq @80076CD0
li r3, 0x1
b @80076F18
@80076CD0
cntlzw r0, r31
li r29, 0x0
srwi r28, r0, 5
cmpwi r28, 0x0
bne @80076D00
mr r3, r31
li r4, 0x0
li r5, 0x6e
li r6, 0x0
bl fn_8012640C
cmpwi r3, 0x0
bne @80076D04
@80076D00
li r29, 0x1
@80076D04
cmpwi r29, 0x0
bne @80076D88
cmpwi r28, 0x0
li r28, 0x0
bne @80076D34
mr r3, r31
li r4, 0x0
li r5, 0x6e
li r6, 0x0
bl fn_8012640C
cmpwi r3, 0x0
bne @80076D38
@80076D34
li r28, 0x1
@80076D38
cmpwi r28, 0x0
beq @80076D48
li r0, 0x0
b @80076D7C
@80076D48
mr r3, r31
li r28, 0x0
bl fn_8011E8DC
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80076D74
mr r3, r31
bl fn_80123FBC
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80076D78
@80076D74
li r28, 0x1
@80076D78
clrlwi r0, r28, 24
@80076D7C
clrlwi r0, r0, 24
cmplwi r0, 0x0
beq @80076D90
@80076D88
li r3, 0x1
b @80076F18
@80076D90
mr r3, r31
bl fn_8011F1A0
clrlwi r0, r3, 16
cmplwi r0, 0x0
bne @80076DAC
li r3, 0x1
b @80076F18
@80076DAC
li r27, 0x0
@80076DB0
mr r3, r30
clrlwi r4, r27, 16
bl fn_8012AC08
mr r26, r3
cmplw r26, r31
beq @80076EA4
cntlzw r0, r26
li r28, 0x0
srwi r29, r0, 5
cmpwi r29, 0x0
bne @80076DF4
li r4, 0x0
li r5, 0x6e
li r6, 0x0
bl fn_8012640C
cmpwi r3, 0x0
bne @80076DF8
@80076DF4
li r28, 0x1
@80076DF8
cmpwi r28, 0x0
bne @80076EA4
cmpwi r29, 0x0
li r29, 0x0
bne @80076E28
mr r3, r26
li r4, 0x0
li r5, 0x6e
li r6, 0x0
bl fn_8012640C
cmpwi r3, 0x0
bne @80076E2C
@80076E28
li r29, 0x1
@80076E2C
cmpwi r29, 0x0
beq @80076E3C
li r0, 0x0
b @80076E70
@80076E3C
mr r3, r26
li r29, 0x0
bl fn_8011E8DC
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80076E68
mr r3, r26
bl fn_80123FBC
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80076E6C
@80076E68
li r29, 0x1
@80076E6C
clrlwi r0, r29, 24
@80076E70
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @80076EA4
mr r3, r31
bl fn_8011F1A0
clrlwi r29, r3, 16
mr r3, r26
bl fn_8011F1A0
clrlwi r0, r3, 16
cmplw r0, r29
bne @80076EA4
li r3, 0x0
b @80076F18
@80076EA4
addi r27, r27, 0x1
cmpwi r27, 0x6
blt @80076DB0
li r3, 0x1
b @80076F18
@80076EB8
mr r4, r5
li r29, 0x0
li r5, 0x3
bl fn_80076F2C
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80076F08
cmplwi r31, 0x0
li r30, 0x0
beq @80076EFC
mr r3, r31
li r4, 0x0
li r5, 0x6e
li r6, 0x0
bl fn_8012640C
cmpwi r3, 0x0
bne @80076F00
@80076EFC
li r30, 0x1
@80076F00
cmpwi r30, 0x0
bne @80076F0C
@80076F08
li r29, 0x1
@80076F0C
clrlwi r3, r29, 24
b @80076F18
@80076F14
li r3, 0x0
@80076F18
lmw r26, 0x8(r1)
lwz r0, 0x24(r1)
mtlr r0
addi r1, r1, 0x20
blr
