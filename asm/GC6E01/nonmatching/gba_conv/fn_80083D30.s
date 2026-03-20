stwu r1, -0x170(r1)
mflr r0
stw r0, 0x174(r1)
stmw r25, 0x154(r1)
mr r27, r3
mr r28, r4
li r31, 0x0
@80083D4C
mr r3, r27
clrlwi r4, r31, 16
bl fn_8012AC08
li r30, 0x0
mr r29, r3
@80083D60
mr r3, r29
clrlwi r4, r30, 16
bl fn_8011F228
bl fn_8011CA34
mr r26, r3
mr r3, r28
li r4, 0x0
li r5, 0x50
bl memset
cmplwi r26, 0x0
beq @80083E98
mr r3, r26
bl fn_8011C7C0
bl fn_800FA280
li r4, 0x0
mr r26, r3
addi r3, r1, 0xac
b @80083DC4
@80083DA8
cmpwi r4, 0x50
bge @80083DC0
mr r0, r4
addi r4, r4, 0x1
slwi r0, r0, 1
sthx r5, r3, r0
@80083DC0
addi r26, r26, 0x2
@80083DC4
lhz r5, 0x0(r26)
cmplwi r5, 0x0
beq @80083DD8
cmplwi r5, 0xffff
bne @80083DA8
@80083DD8
slwi r0, r4, 1
addi r4, r1, 0xac
li r5, 0x0
li r3, 0x0
sthx r5, r4, r0
li r4, 0x5
bl fn_80135938
mr r5, r3
mr r3, r28
addi r4, r1, 0xac
bl fn_800F9AEC
lhz r0, 0x0(r26)
add r25, r28, r3
cmplwi r0, 0xffff
bne @80083E90
li r0, 0xfe
addi r6, r26, 0x3
stb r0, 0x0(r25)
addi r25, r25, 0x1
li r4, 0x0
addi r3, r1, 0x8
b @80083E4C
@80083E30
cmpwi r4, 0x50
bge @80083E48
mr r0, r4
addi r4, r4, 0x1
slwi r0, r0, 1
sthx r5, r3, r0
@80083E48
addi r6, r6, 0x2
@80083E4C
lhz r5, 0x0(r6)
cmplwi r5, 0x0
beq @80083E60
cmplwi r5, 0xffff
bne @80083E30
@80083E60
slwi r0, r4, 1
addi r4, r1, 0x8
li r5, 0x0
li r3, 0x0
sthx r5, r4, r0
li r4, 0x5
bl fn_80135938
mr r5, r3
mr r3, r25
addi r4, r1, 0x8
bl fn_800F9AEC
add r25, r25, r3
@80083E90
li r0, 0xff
stb r0, 0x0(r25)
@80083E98
addi r28, r28, 0x50
addi r30, r30, 0x1
cmpwi r30, 0x4
blt @80083D60
addi r31, r31, 0x1
cmpwi r31, 0x6
blt @80083D4C
li r3, 0x0
lmw r25, 0x154(r1)
lwz r0, 0x174(r1)
mtlr r0
addi r1, r1, 0x170
blr
