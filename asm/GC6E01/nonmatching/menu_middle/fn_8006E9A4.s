stwu r1, -0x910(r1)
mflr r0
stw r0, 0x914(r1)
stw r31, 0x90c(r1)
stw r30, 0x908(r1)
mr r30, r3
mr r31, r4
lbz r0, 0x1(r30)
extsb r0, r0
cmpwi r0, 0x0
beq @8006EE64
b @8006E9D8
b @8006EE64
@8006E9D8
mr r3, r31
bl fn_801091F4
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8006EE64
mr r3, r30
li r4, 0x0
bl fn_801040D0
lha r0, 0x6(r31)
li r5, 0x0
cmpwi r0, 0xd8e
beq @8006EAD8
bge @8006EA64
cmpwi r0, 0x969
beq @8006EC10
bge @8006EA40
cmpwi r0, 0x966
beq @8006ECE8
bge @8006EA34
cmpwi r0, 0x964
beq @8006EDB0
bge @8006ED68
b @8006EE30
@8006EA34
cmpwi r0, 0x968
bge @8006EC58
b @8006ECA0
@8006EA40
cmpwi r0, 0xa0f
beq @8006EB90
bge @8006EA58
cmpwi r0, 0xa0e
bge @8006EBBC
b @8006EE30
@8006EA58
cmpwi r0, 0xd8d
bge @8006EAB8
b @8006EE30
@8006EA64
cmpwi r0, 0xd94
beq @8006EB68
bge @8006EA94
cmpwi r0, 0xd91
beq @8006EB20
bge @8006EA88
cmpwi r0, 0xd90
bge @8006EB00
b @8006EAF8
@8006EA88
cmpwi r0, 0xd93
bge @8006EB48
b @8006EB40
@8006EA94
cmpwi r0, 0xda0
beq @8006EBFC
bge @8006EAAC
cmpwi r0, 0xd96
bge @8006EE30
b @8006EB88
@8006EAAC
cmpwi r0, 0xda2
bge @8006EE30
b @8006EBD0
@8006EAB8
lis r4, 0x51ec
lha r0, 0x0(r3)
subi r3, r4, 0x7ae1
mulhw r0, r3, r0
srawi r0, r0, 5
srwi r3, r0, 31
add r5, r0, r3
b @8006EE30
@8006EAD8
lis r4, 0x6666
lha r0, 0x0(r3)
addi r3, r4, 0x6667
mulhw r0, r3, r0
srawi r0, r0, 2
srwi r3, r0, 31
add r5, r0, r3
b @8006EE30
@8006EAF8
lha r5, 0x0(r3)
b @8006EE30
@8006EB00
lis r4, 0x51ec
lha r0, 0x2(r3)
subi r3, r4, 0x7ae1
mulhw r0, r3, r0
srawi r0, r0, 5
srwi r3, r0, 31
add r5, r0, r3
b @8006EE30
@8006EB20
lis r4, 0x6666
lha r0, 0x2(r3)
addi r3, r4, 0x6667
mulhw r0, r3, r0
srawi r0, r0, 2
srwi r3, r0, 31
add r5, r0, r3
b @8006EE30
@8006EB40
lha r5, 0x2(r3)
b @8006EE30
@8006EB48
lis r4, 0x51ec
lha r0, 0x4(r3)
subi r3, r4, 0x7ae1
mulhw r0, r3, r0
srawi r0, r0, 5
srwi r3, r0, 31
add r5, r0, r3
b @8006EE30
@8006EB68
lis r4, 0x6666
lha r0, 0x4(r3)
addi r3, r4, 0x6667
mulhw r0, r3, r0
srawi r0, r0, 2
srwi r3, r0, 31
add r5, r0, r3
b @8006EE30
@8006EB88
lha r5, 0x4(r3)
b @8006EE30
@8006EB90
lha r0, 0x14(r3)
lis r3, 0x6666
addi r4, r3, 0x6667
srawi r3, r0, 31
xor r0, r3, r0
subf r0, r3, r0
mulhw r0, r4, r0
srawi r0, r0, 2
srwi r3, r0, 31
add r5, r0, r3
b @8006EE30
@8006EBBC
lha r3, 0x14(r3)
srawi r0, r3, 31
xor r5, r0, r3
subf r5, r0, r5
b @8006EE30
@8006EBD0
lha r0, 0x16(r3)
lis r3, 0x6666
addi r4, r3, 0x6667
srawi r3, r0, 31
xor r0, r3, r0
subf r0, r3, r0
mulhw r0, r4, r0
srawi r0, r0, 2
srwi r3, r0, 31
add r5, r0, r3
b @8006EE30
@8006EBFC
lha r3, 0x16(r3)
srawi r0, r3, 31
xor r5, r0, r3
subf r5, r0, r5
b @8006EE30
@8006EC10
lwz r31, 0x64(r31)
addi r3, r1, 0x288
li r4, lbl_8047C068@sda21
li r5, 0x32
crclr 6
bl fn_800C8520
addi r3, r1, 0x808
addi r4, r1, 0x288
bl fn_800F9D04
addi r4, r1, 0x808
li r3, 0x37
bl fn_80132A38
mr r5, r31
li r3, 0xa
li r4, 0x0
li r6, 0xcf
bl fn_800FB680
b @8006EE64
@8006EC58
lwz r31, 0x64(r31)
addi r3, r1, 0x208
li r4, lbl_8047C068@sda21
li r5, 0x32
crclr 6
bl fn_800C8520
addi r3, r1, 0x708
addi r4, r1, 0x208
bl fn_800F9D04
addi r4, r1, 0x708
li r3, 0x37
bl fn_80132A38
mr r5, r31
li r3, 0xa
li r4, 0x0
li r6, 0xcf
bl fn_800FB680
b @8006EE64
@8006ECA0
lwz r31, 0x64(r31)
addi r3, r1, 0x188
li r4, lbl_8047C068@sda21
li r5, 0x32
crclr 6
bl fn_800C8520
addi r3, r1, 0x608
addi r4, r1, 0x188
bl fn_800F9D04
addi r4, r1, 0x608
li r3, 0x37
bl fn_80132A38
mr r5, r31
li r3, 0xa
li r4, 0x0
li r6, 0xcf
bl fn_800FB680
b @8006EE64
@8006ECE8
lwz r31, 0x64(r31)
li r3, 0x3
bl fn_8006B3C8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8006ED08
li r5, 0x32
b @8006ED0C
@8006ED08
li r5, -0x1
@8006ED0C
cmpwi r5, 0x0
blt @8006ED28
addi r3, r1, 0x108
li r4, lbl_8047C068@sda21
crclr 6
bl fn_800C8520
b @8006ED38
@8006ED28
addi r3, r1, 0x108
li r4, lbl_8047C070@sda21
crclr 6
bl fn_800C8520
@8006ED38
addi r3, r1, 0x508
addi r4, r1, 0x108
bl fn_800F9D04
addi r4, r1, 0x508
li r3, 0x37
bl fn_80132A38
mr r5, r31
li r3, 0xa
li r4, 0x0
li r6, 0xcf
bl fn_800FB680
b @8006EE64
@8006ED68
lwz r31, 0x64(r31)
addi r3, r1, 0x88
li r4, lbl_8047C068@sda21
li r5, 0x64
crclr 6
bl fn_800C8520
addi r3, r1, 0x408
addi r4, r1, 0x88
bl fn_800F9D04
addi r4, r1, 0x408
li r3, 0x37
bl fn_80132A38
mr r5, r31
li r3, 0xa
li r4, 0x0
li r6, 0xcf
bl fn_800FB680
b @8006EE64
@8006EDB0
lwz r31, 0x64(r31)
li r3, 0x5
bl fn_8006B3C8
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @8006EDD0
li r5, 0x64
b @8006EDD4
@8006EDD0
li r5, -0x1
@8006EDD4
cmpwi r5, 0x0
blt @8006EDF0
addi r3, r1, 0x8
li r4, lbl_8047C068@sda21
crclr 6
bl fn_800C8520
b @8006EE00
@8006EDF0
addi r3, r1, 0x8
li r4, lbl_8047C070@sda21
crclr 6
bl fn_800C8520
@8006EE00
addi r3, r1, 0x308
addi r4, r1, 0x8
bl fn_800F9D04
addi r4, r1, 0x308
li r3, 0x37
bl fn_80132A38
mr r5, r31
li r3, 0xa
li r4, 0x0
li r6, 0xcf
bl fn_800FB680
b @8006EE64
@8006EE30
lis r4, 0xcccd
li r3, 0x34
subi r0, r4, 0x3333
mulhwu r0, r0, r5
srwi r0, r0, 3
mulli r0, r0, 0xa
subf r4, r0, r5
bl fn_80132A38
lwz r5, 0x64(r31)
li r3, 0x0
li r4, 0x0
li r6, 0xc9
bl fn_800FB680
@8006EE64
lwz r0, 0x914(r1)
lwz r31, 0x90c(r1)
lwz r30, 0x908(r1)
mtlr r0
addi r1, r1, 0x910
blr
