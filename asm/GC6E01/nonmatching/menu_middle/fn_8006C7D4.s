stwu r1, -0x1a0(r1)
mflr r0
stw r0, 0x1a4(r1)
stw r31, 0x19c(r1)
stw r30, 0x198(r1)
stw r29, 0x194(r1)
stw r28, 0x190(r1)
mr r28, r4
lha r3, 0x6(r28)
subi r0, r3, 0xec2
cmplwi r0, 0x23
bgt @8006CCA0
lis r3, jumptable_802EDF20@ha
slwi r0, r0, 2
addi r3, r3, jumptable_802EDF20@l
lwzx r0, r3, r0
mtctr r0
bctr
li r0, 0x2
li r30, 0x0
b @8006C9D0
li r0, 0x2
li r30, 0x1
b @8006C9D0
li r0, 0x2
li r30, 0x2
b @8006C9D0
li r0, 0x2
li r30, 0x3
b @8006C9D0
li r0, 0x2
li r30, 0x0
b @8006C9D0
li r0, 0x2
li r30, 0x1
b @8006C9D0
li r0, 0x2
li r30, 0x2
b @8006C9D0
li r0, 0x2
li r30, 0x3
b @8006C9D0
li r0, 0x2
li r30, 0x0
b @8006C9D0
li r0, 0x2
li r30, 0x1
b @8006C9D0
li r0, 0x2
li r30, 0x2
b @8006C9D0
li r0, 0x2
li r30, 0x3
b @8006C9D0
li r0, 0x1
li r30, 0x0
b @8006C9D0
li r0, 0x1
li r30, 0x1
b @8006C9D0
li r0, 0x1
li r30, 0x2
b @8006C9D0
li r0, 0x1
li r30, 0x3
b @8006C9D0
li r0, 0x1
li r30, 0x0
b @8006C9D0
li r0, 0x1
li r30, 0x1
b @8006C9D0
li r0, 0x1
li r30, 0x2
b @8006C9D0
li r0, 0x1
li r30, 0x3
b @8006C9D0
li r0, 0x1
li r30, 0x0
b @8006C9D0
li r0, 0x1
li r30, 0x1
b @8006C9D0
li r0, 0x1
li r30, 0x2
b @8006C9D0
li r0, 0x1
li r30, 0x3
b @8006C9D0
li r0, 0x3
li r30, 0x0
b @8006C9D0
li r0, 0x3
li r30, 0x1
b @8006C9D0
li r0, 0x3
li r30, 0x2
b @8006C9D0
li r0, 0x3
li r30, 0x3
b @8006C9D0
li r0, 0x3
li r30, 0x0
b @8006C9D0
li r0, 0x3
li r30, 0x1
b @8006C9D0
li r0, 0x3
li r30, 0x2
b @8006C9D0
li r0, 0x3
li r30, 0x3
b @8006C9D0
li r0, 0x3
li r30, 0x0
b @8006C9D0
li r0, 0x3
li r30, 0x1
b @8006C9D0
li r0, 0x3
li r30, 0x2
b @8006C9D0
li r0, 0x3
li r30, 0x3
b @8006C9D0
b @8006CCA0
@8006C9D0
cmpwi r0, 0x2
beq @8006CA40
bge @8006C9EC
cmpwi r0, 0x0
beq @8006CCA0
bge @8006C9F8
b @8006CCA0
@8006C9EC
cmpwi r0, 0x4
bge @8006CCA0
b @8006CB80
@8006C9F8
lwz r29, 0x64(r28)
li r3, 0x0
li r4, 0xe
bl fn_80129280
mulli r4, r30, 0x1660
addi r0, r4, 0x64ec
add r0, r3, r0
mr r3, r0
bl fn_8012AC54
mr r4, r3
li r3, 0x37
bl fn_80132A38
mr r5, r29
li r3, 0x0
li r4, 0x0
li r6, 0xd0
bl fn_800FB680
b @8006CCA0
@8006CA40
li r31, 0x0
li r3, 0x0
li r4, 0xe
bl fn_80129280
mulli r4, r30, 0x1660
addi r29, r4, 0x59a8
add r29, r3, r29
mr r3, r29
bl fn_8006A7E8
cmpwi r3, 0x1
beq @8006CA90
bge @8006CA7C
cmpwi r3, 0x0
bge @8006CA88
b @8006CB08
@8006CA7C
cmpwi r3, 0x3
bge @8006CB08
b @8006CACC
@8006CA88
li r0, 0x0
b @8006CB0C
@8006CA90
addi r3, r29, 0xb44
bl fn_8012AA2C
clrlwi r0, r3, 24
cmpwi r0, 0x1
beq @8006CAC4
bge @8006CAB4
cmpwi r0, 0x0
bge @8006CABC
b @8006CB08
@8006CAB4
cmpwi r0, 0x3
b @8006CB08
@8006CABC
li r0, 0x1
b @8006CB0C
@8006CAC4
li r0, 0x2
b @8006CB0C
@8006CACC
addi r3, r29, 0xb44
bl fn_8012AA2C
clrlwi r0, r3, 24
cmpwi r0, 0x1
beq @8006CB00
bge @8006CAF0
cmpwi r0, 0x0
bge @8006CAF8
b @8006CB08
@8006CAF0
cmpwi r0, 0x3
b @8006CB08
@8006CAF8
li r0, 0x3
b @8006CB0C
@8006CB00
li r0, 0x4
b @8006CB0C
@8006CB08
li r0, 0x1
@8006CB0C
cmpwi r0, 0x2
beq @8006CB48
bge @8006CB28
cmpwi r0, 0x0
beq @8006CB38
bge @8006CB40
b @8006CB5C
@8006CB28
cmpwi r0, 0x4
beq @8006CB58
bge @8006CB5C
b @8006CB50
@8006CB38
li r31, 0x29f
b @8006CB5C
@8006CB40
li r31, 0x2a1
b @8006CB5C
@8006CB48
li r31, 0x2a2
b @8006CB5C
@8006CB50
li r31, 0x2a3
b @8006CB5C
@8006CB58
li r31, 0x2a0
@8006CB5C
cmplwi r31, 0x0
beq @8006CCA0
mr r3, r31
bl fn_8005D858
mr r0, r3
mr r3, r28
mr r4, r0
bl fn_80071318
b @8006CCA0
@8006CB80
lwz r31, 0x64(r28)
li r3, 0x0
li r4, 0xe
bl fn_80129280
mulli r4, r30, 0x1660
addi r0, r4, 0x64ec
add r0, r3, r0
mr r3, r0
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
li r6, 0xd0
bl fn_800FB680
@8006CCA0
lwz r0, 0x1a4(r1)
lwz r31, 0x19c(r1)
lwz r30, 0x198(r1)
lwz r29, 0x194(r1)
lwz r28, 0x190(r1)
mtlr r0
addi r1, r1, 0x1a0
blr
