    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r3
    lwz r0, 0x4(r31)
    cmpwi r0, 0x10b
    beq @L_8000DCD8
    bge @L_8000DCF4
    cmpwi r0, 0x10a
    bge @L_8000DCB8
    b @L_8000DCF4
    @L_8000DCB8:
    bl fn_80265B74
    fctiwz f0, f1
    mr r3, r31
    li r4, 0x0
    stfd f0, 0x8(r1)
    lwz r5, 0xc(r1)
    bl fn_801040B8
    b @L_8000DCF4
    @L_8000DCD8:
    bl fn_8026595C
    fctiwz f0, f1
    mr r3, r31
    li r4, 0x0
    stfd f0, 0x8(r1)
    lwz r5, 0xc(r1)
    bl fn_801040B8
    @L_8000DCF4:
    li r3, 0x0
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
