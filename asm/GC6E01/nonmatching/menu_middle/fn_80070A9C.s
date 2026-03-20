stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
mr r31, r4
lbz r0, 0x1(r3)
extsb r0, r0
cmpwi r0, 0x3
bge @80070D1C
cmpwi r0, 0x0
bge @80070ACC
b @80070D1C
@80070ACC
lwz r0, 0x4c(r31)
li r4, 0x0
stw r0, lbl_8047A5E8@sda21(r0)
lwz r0, 0x4c(r31)
stw r0, lbl_8047A5EC@sda21(r0)
bl fn_801040D0
subi r0, r3, 0xa8
cmplwi r0, 0x4d
bgt @80070D1C
lis r3, jumptable_802EE31C@ha
slwi r0, r0, 2
addi r3, r3, jumptable_802EE31C@l
lwzx r0, r3, r0
mtctr r0
bctr
li r0, 0x0
stw r0, lbl_8047A5E8@sda21(r0)
b @80070D1C
li r3, 0x3d3a
li r0, 0x0
stw r3, lbl_8047A5E8@sda21(r0)
stw r0, lbl_8047A5EC@sda21(r0)
b @80070D1C
li r3, 0x3d3a
li r0, 0x3f3e
stw r3, lbl_8047A5E8@sda21(r0)
stw r0, lbl_8047A5EC@sda21(r0)
b @80070D1C
li r3, 0x3d3a
li r0, 0x3da6
stw r3, lbl_8047A5E8@sda21(r0)
stw r0, lbl_8047A5EC@sda21(r0)
b @80070D1C
li r3, 0x3d3a
li r0, 0x3d50
stw r3, lbl_8047A5E8@sda21(r0)
stw r0, lbl_8047A5EC@sda21(r0)
b @80070D1C
li r3, 0x3d3a
li r0, 0x3d2d
stw r3, lbl_8047A5E8@sda21(r0)
stw r0, lbl_8047A5EC@sda21(r0)
b @80070D1C
li r3, 0x3d6e
li r0, 0x0
stw r3, lbl_8047A5E8@sda21(r0)
stw r0, lbl_8047A5EC@sda21(r0)
b @80070D1C
li r3, 0x3d3a
li r0, 0x3d42
stw r3, lbl_8047A5E8@sda21(r0)
stw r0, lbl_8047A5EC@sda21(r0)
b @80070D1C
li r3, 0x4274
li r0, 0x0
stw r3, lbl_8047A5E8@sda21(r0)
stw r0, lbl_8047A5EC@sda21(r0)
b @80070D1C
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x0(r3)
cmpwi r0, 0x0
bne @80070BE0
li r3, 0x4237
li r0, 0x0
stw r3, lbl_8047A5E8@sda21(r0)
stw r0, lbl_8047A5EC@sda21(r0)
b @80070D1C
@80070BE0
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x8(r3)
cmpwi r0, 0x3
beq @80070C60
bge @80070C14
cmpwi r0, 0x1
beq @80070C38
bge @80070C4C
cmpwi r0, 0x0
bge @80070C24
b @80070D1C
@80070C14
cmpwi r0, 0x5
beq @80070C88
bge @80070D1C
b @80070C74
@80070C24
li r3, 0x3d7c
li r0, 0x0
stw r3, lbl_8047A5E8@sda21(r0)
stw r0, lbl_8047A5EC@sda21(r0)
b @80070D1C
@80070C38
li r3, 0x3d7d
li r0, 0x0
stw r3, lbl_8047A5E8@sda21(r0)
stw r0, lbl_8047A5EC@sda21(r0)
b @80070D1C
@80070C4C
li r3, 0x3d7e
li r0, 0x0
stw r3, lbl_8047A5E8@sda21(r0)
stw r0, lbl_8047A5EC@sda21(r0)
b @80070D1C
@80070C60
li r3, 0x3d7f
li r0, 0x0
stw r3, lbl_8047A5E8@sda21(r0)
stw r0, lbl_8047A5EC@sda21(r0)
b @80070D1C
@80070C74
li r3, 0x3d80
li r0, 0x0
stw r3, lbl_8047A5E8@sda21(r0)
stw r0, lbl_8047A5EC@sda21(r0)
b @80070D1C
@80070C88
li r3, 0x3d81
li r0, 0x0
stw r3, lbl_8047A5E8@sda21(r0)
stw r0, lbl_8047A5EC@sda21(r0)
b @80070D1C
li r3, 0x0
li r4, 0xe
bl fn_80129280
lwz r0, 0x0(r3)
cmpwi r0, 0x2
beq @80070D0C
bge @80070CC8
cmpwi r0, 0x0
beq @80070CE4
bge @80070CF8
b @80070D1C
@80070CC8
cmpwi r0, 0x4
bge @80070D1C
li r3, 0x3d6e
li r0, 0x0
stw r3, lbl_8047A5E8@sda21(r0)
stw r0, lbl_8047A5EC@sda21(r0)
b @80070D1C
@80070CE4
li r3, 0x3d3a
li r0, 0x3dab
stw r3, lbl_8047A5E8@sda21(r0)
stw r0, lbl_8047A5EC@sda21(r0)
b @80070D1C
@80070CF8
li r3, 0x3d3a
li r0, 0x423c
stw r3, lbl_8047A5E8@sda21(r0)
stw r0, lbl_8047A5EC@sda21(r0)
b @80070D1C
@80070D0C
li r3, 0x3d3a
li r0, 0x3d2d
stw r3, lbl_8047A5E8@sda21(r0)
stw r0, lbl_8047A5EC@sda21(r0)
@80070D1C
lwz r4, lbl_8047A5E8@sda21(r0)
mr r3, r31
neg r0, r4
or r0, r0, r4
srwi r4, r0, 31
bl fn_80109220
lha r0, 0x6(r31)
cmpwi r0, 0x89b
beq @80070D5C
bge @80070D50
cmpwi r0, 0x80a
beq @80070D70
b @80070D70
@80070D50
cmpwi r0, 0x93e
beq @80070D68
b @80070D70
@80070D5C
lwz r0, lbl_8047A5E8@sda21(r0)
stw r0, 0x4c(r31)
b @80070D70
@80070D68
lwz r0, lbl_8047A5EC@sda21(r0)
stw r0, 0x4c(r31)
@80070D70
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
mtlr r0
addi r1, r1, 0x10
blr
