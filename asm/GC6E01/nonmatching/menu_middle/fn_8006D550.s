stwu r1, -0x1b0(r1)
mflr r0
stw r0, 0x1b4(r1)
stmw r26, 0x198(r1)
mr r31, r4
mr r3, r31
bl fn_801091F4
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8006D92C
lha r3, 0x6(r31)
li r28, 0x0
subi r0, r3, 0xa4f
cmplwi r0, 0x27
bgt @8006D92C
lis r3, jumptable_802EDFCC@ha
slwi r0, r0, 2
addi r3, r3, jumptable_802EDFCC@l
lwzx r0, r3, r0
mtctr r0
bctr
li r30, 0x2
li r26, 0x0
b @8006D638
li r30, 0x2
li r26, 0x1
b @8006D638
li r30, 0x2
li r26, 0x2
b @8006D638
li r30, 0x2
li r26, 0x3
b @8006D638
li r30, 0x1
li r26, 0x0
b @8006D638
li r30, 0x1
li r26, 0x1
b @8006D638
li r30, 0x1
li r26, 0x2
b @8006D638
li r30, 0x1
li r26, 0x3
b @8006D638
li r30, 0x3
li r26, 0x0
b @8006D638
li r30, 0x3
li r26, 0x1
b @8006D638
li r30, 0x3
li r26, 0x2
b @8006D638
li r30, 0x3
li r26, 0x3
b @8006D638
b @8006D92C
@8006D638
li r27, 0x0
li r29, 0x0
@8006D640
li r3, 0x0
li r4, 0xe
bl fn_80129280
addi r0, r29, 0x59cc
lwzx r3, r3, r0
bl fn_8006B154
cmpw r26, r3
bne @8006D67C
li r3, 0x0
li r4, 0xe
bl fn_80129280
mulli r4, r27, 0x1660
addi r28, r4, 0x59a8
add r28, r3, r28
b @8006D68C
@8006D67C
addi r29, r29, 0x1660
addi r27, r27, 0x1
cmplwi r27, 0x4
blt @8006D640
@8006D68C
cmplwi r28, 0x0
beq @8006D92C
cmpwi r30, 0x2
beq @8006D6E8
bge @8006D6AC
cmpwi r30, 0x1
bge @8006D6B8
b @8006D918
@8006D6AC
cmpwi r30, 0x4
bge @8006D918
b @8006D7F4
@8006D6B8
lwz r29, 0x64(r31)
addi r3, r28, 0xb44
bl fn_8012AC54
mr r4, r3
li r3, 0x37
bl fn_80132A38
mr r5, r29
li r3, 0x0
li r4, 0x0
li r6, 0xcf
bl fn_800FB680
b @8006D92C
@8006D6E8
lwz r31, 0x64(r31)
addi r3, r28, 0xb44
bl fn_8012AC3C
lis r7, 0xcccd
lis r6, 0x51ec
clrlwi r0, r3, 16
lis r5, 0x1062
subi r9, r7, 0x3333
subi r7, r6, 0x7ae1
mulhwu r8, r9, r0
lis r4, 0xd1b7
addi r6, r5, 0x4dd3
addi r5, r4, 0x1759
lis r4, lbl_802686D0@ha
addi r3, r1, 0x10
mulhwu r7, r7, r0
mr r29, r8
srwi r30, r8, 3
srwi r29, r29, 3
addi r4, r4, lbl_802686D0@l
mulhwu r6, r6, r0
srwi r12, r7, 5
mulhwu r5, r5, r0
srwi r11, r6, 6
mulhwu r8, r9, r30
srwi r10, r5, 13
mulhwu r7, r9, r12
srwi r8, r8, 3
mulhwu r6, r9, r11
srwi r7, r7, 3
mulhwu r5, r9, r10
srwi r6, r6, 3
mulli r9, r29, 0xa
srwi r5, r5, 3
subf r0, r9, r0
clrlwi r9, r0, 24
mulli r0, r7, 0xa
stb r9, 0x8(r1)
mulli r8, r8, 0xa
subf r0, r0, r12
subf r7, r8, r30
clrlwi r8, r7, 24
clrlwi r7, r0, 24
stb r8, 0x9(r1)
mulli r0, r5, 0xa
stb r7, 0xa(r1)
mulli r6, r6, 0xa
subf r0, r0, r10
subf r5, r6, r11
clrlwi r6, r5, 24
clrlwi r5, r0, 24
stb r6, 0xb(r1)
stb r5, 0xc(r1)
crclr 6
bl fn_800C8520
addi r3, r1, 0x90
addi r4, r1, 0x10
bl fn_800F9D04
addi r4, r1, 0x90
li r3, 0x37
bl fn_80132A38
mr r5, r31
li r3, 0x0
li r4, 0x0
li r6, 0xcf
bl fn_800FB680
b @8006D92C
@8006D7F4
mr r3, r28
li r26, 0x0
bl fn_8006A7E8
cmpwi r3, 0x1
beq @8006D82C
bge @8006D818
cmpwi r3, 0x0
bge @8006D824
b @8006D8A4
@8006D818
cmpwi r3, 0x3
bge @8006D8A4
b @8006D868
@8006D824
li r0, 0x0
b @8006D8A8
@8006D82C
addi r3, r28, 0xb44
bl fn_8012AA2C
clrlwi r0, r3, 24
cmpwi r0, 0x1
beq @8006D860
bge @8006D850
cmpwi r0, 0x0
bge @8006D858
b @8006D8A4
@8006D850
cmpwi r0, 0x3
b @8006D8A4
@8006D858
li r0, 0x1
b @8006D8A8
@8006D860
li r0, 0x2
b @8006D8A8
@8006D868
addi r3, r28, 0xb44
bl fn_8012AA2C
clrlwi r0, r3, 24
cmpwi r0, 0x1
beq @8006D89C
bge @8006D88C
cmpwi r0, 0x0
bge @8006D894
b @8006D8A4
@8006D88C
cmpwi r0, 0x3
b @8006D8A4
@8006D894
li r0, 0x3
b @8006D8A8
@8006D89C
li r0, 0x4
b @8006D8A8
@8006D8A4
li r0, 0x1
@8006D8A8
cmpwi r0, 0x2
beq @8006D8E4
bge @8006D8C4
cmpwi r0, 0x0
beq @8006D8D4
bge @8006D8DC
b @8006D8F8
@8006D8C4
cmpwi r0, 0x4
beq @8006D8F4
bge @8006D8F8
b @8006D8EC
@8006D8D4
li r26, 0x2ba
b @8006D8F8
@8006D8DC
li r26, 0x2bc
b @8006D8F8
@8006D8E4
li r26, 0x2b5
b @8006D8F8
@8006D8EC
li r26, 0x2bb
b @8006D8F8
@8006D8F4
li r26, 0x2b4
@8006D8F8
cmplwi r26, 0x0
beq @8006D92C
mr r3, r26
bl fn_8005D858
mr r4, r3
mr r3, r31
bl fn_80071318
b @8006D92C
@8006D918
lis r3, lbl_80268680@ha
li r4, 0xae6
addi r3, r3, lbl_80268680@l
li r5, lbl_8047C064@sda21
bl fn_80196E10
@8006D92C
lmw r26, 0x198(r1)
lwz r0, 0x1b4(r1)
mtlr r0
addi r1, r1, 0x1b0
blr
