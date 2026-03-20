stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
stw r30, 0x8(r1)
mr r30, r3
mr r31, r6
clrlwi r0, r4, 24
cmplwi r0, 0x0
bne @80079CBC
clrlwi r0, r5, 24
cmplwi r0, 0x0
bne @80079CBC
li r3, 0x43d2
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x1
bl fn_801069FC
li r3, 0x43d3
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x1
bl fn_801069FC
cmpwi r30, 0x0
bne @80079CA4
li r3, 0x2
li r4, 0x44cf
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
@80079CA4
li r3, 0xef
bl fn_80102510
li r0, 0x1
li r3, 0x0
stw r0, lbl_8047A638@sda21(r0)
b @80079EDC
@80079CBC
clrlwi r0, r4, 24
cmplwi r0, 0x0
bne @80079D1C
li r3, 0x43d2
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x1
bl fn_801069FC
cmpwi r30, 0x0
bne @80079D04
li r3, 0x2
li r4, 0x44cf
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
@80079D04
li r3, 0xef
bl fn_80102510
li r0, 0x1
li r3, 0x0
stw r0, lbl_8047A638@sda21(r0)
b @80079EDC
@80079D1C
clrlwi r0, r5, 24
cmplwi r0, 0x0
bne @80079D7C
li r3, 0x43d3
li r4, 0x1
li r5, 0x0
bl fn_801067E8
li r3, 0x1
bl fn_801069FC
cmpwi r30, 0x0
bne @80079D64
li r3, 0x2
li r4, 0x44cf
li r5, 0x1
li r6, 0x0
bl fn_80106D3C
li r3, 0x1
bl fn_801069FC
@80079D64
li r3, 0xef
bl fn_80102510
li r0, 0x1
li r3, 0x0
stw r0, lbl_8047A638@sda21(r0)
b @80079EDC
@80079D7C
lbz r0, lbl_8047A632@sda21(r0)
cmplwi r0, 0x0
beq @80079DF4
li r3, 0x2d
li r4, 0x47
bl fn_80132A38
li r3, 0x3ca
li r4, 0x0
li r5, 0xff
bl fn_80165668
cmpwi r30, 0x1
beq @80079DBC
bge @80079DEC
cmpwi r30, 0x0
bge @80079DD0
b @80079DEC
@80079DBC
li r3, 0x43ad
li r4, 0x1
li r5, 0x0
bl fn_801067E8
b @80079DEC
@80079DD0
mr r4, r31
li r3, 0x4d
bl fn_80132A38
li r3, 0x4436
li r4, 0x1
li r5, 0x0
bl fn_801067E8
@80079DEC
li r3, 0x1
bl fn_801069FC
@80079DF4
lbz r0, lbl_8047A631@sda21(r0)
cmplwi r0, 0x0
beq @80079E60
li r3, 0x3d2
li r4, 0x0
li r5, 0xff
bl fn_80165668
cmpwi r30, 0x1
beq @80079E28
bge @80079E58
cmpwi r30, 0x0
bge @80079E3C
b @80079E58
@80079E28
li r3, 0x4437
li r4, 0x1
li r5, 0x0
bl fn_801067E8
b @80079E58
@80079E3C
mr r4, r31
li r3, 0x4d
bl fn_80132A38
li r3, 0x443b
li r4, 0x1
li r5, 0x0
bl fn_801067E8
@80079E58
li r3, 0x1
bl fn_801069FC
@80079E60
lbz r0, lbl_8047A630@sda21(r0)
cmplwi r0, 0x0
beq @80079ED8
li r3, 0x2d
li r4, 0x1
bl fn_80132A38
li r3, 0x3ca
li r4, 0x0
li r5, 0xff
bl fn_80165668
cmpwi r30, 0x1
beq @80079EA0
bge @80079ED0
cmpwi r30, 0x0
bge @80079EB4
b @80079ED0
@80079EA0
li r3, 0x43ad
li r4, 0x1
li r5, 0x0
bl fn_801067E8
b @80079ED0
@80079EB4
mr r4, r31
li r3, 0x4d
bl fn_80132A38
li r3, 0x4436
li r4, 0x1
li r5, 0x0
bl fn_801067E8
@80079ED0
li r3, 0x1
bl fn_801069FC
@80079ED8
li r3, 0x1
@80079EDC
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
lwz r30, 0x8(r1)
mtlr r0
addi r1, r1, 0x10
blr
