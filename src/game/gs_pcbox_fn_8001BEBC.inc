    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r31, r3
    lis r3, lbl_803A1D40@ha
    lhz r29, 0x0(r31)
    addi r3, r3, lbl_803A1D40@l
    li r30, 0x0
    lwz r0, 0x8(r3)
    lwz r3, 0xc(r3)
    cmpwi r0, 0x1
    beq @L_8001BFB8
    bge @L_8001BF08
    cmpwi r0, 0x0
    bge @L_8001BF14
    b @L_8001C020
    @L_8001BF08:
    cmpwi r0, 0x3
    bge @L_8001C020
    b @L_8001C014
    @L_8001BF14:
    cmplwi r29, 0x6
    blt @L_8001BF20
    b @L_8001C038
    @L_8001BF20:
    cmpwi r0, 0x1
    beq @L_8001BF60
    bge @L_8001BF94
    cmpwi r0, 0x0
    bge @L_8001BF38
    b @L_8001BF94
    @L_8001BF38:
    li r3, 0x8ae
    bl fn_801906A0
    cmplwi r3, 0x0
    bne @L_8001BF58
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    b @L_8001BF98
    @L_8001BF58:
    bl fn_8006AEEC
    b @L_8001BF98
    @L_8001BF60:
    cmplwi r3, 0x0
    bne @L_8001BF70
    li r3, 0x0
    bl fn_801F2A7C
    @L_8001BF70:
    cmplwi r3, 0x0
    bne @L_8001BF80
    li r3, 0x0
    b @L_8001BF98
    @L_8001BF80:
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    b @L_8001BF98
    @L_8001BF94:
    li r3, 0x0
    @L_8001BF98:
    cmplwi r3, 0x0
    bne @L_8001BFA8
    li r30, 0x0
    b @L_8001C038
    @L_8001BFA8:
    mr r4, r29
    bl fn_8012AC08
    mr r30, r3
    b @L_8001C020
    @L_8001BFB8:
    cmplwi r29, 0x6
    blt @L_8001BFC4
    b @L_8001C038
    @L_8001BFC4:
    cmplwi r3, 0x0
    bne @L_8001BFD4
    li r3, 0x0
    bl fn_801F2A7C
    @L_8001BFD4:
    cmplwi r3, 0x0
    bne @L_8001BFE4
    li r30, 0x0
    b @L_8001C038
    @L_8001BFE4:
    mr r4, r29
    bl fn_801F986C
    cmplwi r3, 0x0
    bne @L_8001BFFC
    li r30, 0x0
    b @L_8001C038
    @L_8001BFFC:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r30, r3
    b @L_8001C020
    @L_8001C014:
    cmplwi r29, 0x1e
    blt @L_8001C020
    b @L_8001C038
    @L_8001C020:
    mr r3, r30
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8001C038
    li r30, 0x0
    @L_8001C038:
    lhz r4, 0x0(r31)
    mr r3, r30
    addi r0, r4, 0x1
    sth r0, 0x0(r31)
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
