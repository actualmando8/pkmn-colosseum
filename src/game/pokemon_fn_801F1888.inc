    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x1a
    stw r0, 0x14(r1)
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r0, r3, 16
    cmplwi r0, 0xa
    beq @L_801F18C0
    cmplwi r0, 0x11
    beq @L_801F18C0
    cmplwi r0, 0xb
    bne @L_801F18C8
    @L_801F18C0:
    li r3, 0x1
    b @L_801F18CC
    @L_801F18C8:
    li r3, 0x0
    @L_801F18CC:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
