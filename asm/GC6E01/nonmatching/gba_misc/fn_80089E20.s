stwu r1, -0x90(r1)
mflr r0
stw r0, 0x94(r1)
stw r31, 0x8c(r1)
stw r30, 0x88(r1)
stw r29, 0x84(r1)
mr r30, r3
mr r31, r4
mr r29, r6
rlwinm r0, r5, 0, 16, 23
rlwinm r4, r5, 0, 8, 15
slwi r3, r5, 24
srwi r5, r5, 24
slwi r0, r0, 8
srwi r4, r4, 8
or r0, r3, r0
mr r3, r31
or r0, r4, r0
or r0, r5, r0
stw r0, 0x8(r1)
bl fn_8011F5C8
clrlwi r3, r3, 16
slwi r0, r29, 16
or r6, r0, r3
mr r3, r31
rlwinm r0, r6, 0, 16, 23
rlwinm r5, r6, 0, 8, 15
slwi r4, r6, 24
slwi r0, r0, 8
srwi r6, r6, 24
srwi r5, r5, 8
or r0, r4, r0
or r0, r5, r0
or r0, r6, r0
stw r0, 0xc(r1)
bl fn_8011F5C8
clrlwi r0, r3, 16
cmplwi r0, 0x181
bne @80089EDC
mr r3, r31
bl fn_8011E868
clrlwi r0, r3, 24
mr r3, r31
cntlzw r0, r0
srwi r0, r0, 5
clrlwi r4, r0, 24
bl fn_8011D504
@80089EDC
mr r3, r31
addi r4, r1, 0x10
bl fn_8008AE18
addi r31, r1, 0x74
li r4, 0x0
mr r3, r31
li r5, 0xc
bl memset
li r29, 0x0
@80089F00
mr r3, r29
bl fn_80265F14
stb r3, 0x0(r31)
addi r31, r31, 0x1
addi r29, r29, 0x1
cmpwi r29, 0xb
blt @80089F00
subi r3, r30, 0x1
addi r4, r1, 0x8
bl fn_800726A8
slwi r0, r30, 1
li r4, lbl_8047A684@sda21
add r4, r4, r0
li r0, 0x0
sth r0, -0x2(r4)
lwz r0, 0x94(r1)
lwz r31, 0x8c(r1)
lwz r30, 0x88(r1)
lwz r29, 0x84(r1)
mtlr r0
addi r1, r1, 0x90
blr
