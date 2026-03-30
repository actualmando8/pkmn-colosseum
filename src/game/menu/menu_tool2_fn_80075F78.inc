    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lbz r0, 0x95(r3)
    extsb r0, r0
    cmpwi r0, 0x1
    beq @L_80075FB8
    bge @L_80075FA4
    cmpwi r0, 0x0
    bge @L_80075FB0
    b @L_80075FC8
    @L_80075FA4:
    cmpwi r0, 0x3
    bge @L_80075FC8
    b @L_80075FC0
    @L_80075FB0:
    li r3, 0x43bc
    b @L_80075FCC
    @L_80075FB8:
    li r3, 0x43ba
    b @L_80075FCC
    @L_80075FC0:
    li r3, 0x43be
    b @L_80075FCC
    @L_80075FC8:
    li r3, 0x1
    @L_80075FCC:
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
