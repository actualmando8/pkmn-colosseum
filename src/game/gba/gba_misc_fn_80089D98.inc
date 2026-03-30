    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    subi r3, r31, 0x1
    bl fn_80072548
    cmpwi r3, 0x0
    bge @L_80089DD4
    rlwinm r0, r31, 1, 0, 30
    addi r4, r13, -24988
    add r4, r4, r0
    li r0, 0x0
    sth r0, -0x2(r4)
    b @L_80089E0C
@L_80089DD4:
    cmpwi r3, 0x1
    beq @L_80089DE4
    cmpwi r3, 0x2
    bne @L_80089E0C
@L_80089DE4:
    rlwinm r0, r31, 1, 0, 30
    addi r4, r13, -24988
    add r5, r4, r0
    lhz r4, -0x2(r5)
    addi r4, r4, 0x1
    rlwinm r0, r4, 0, 16, 31
    sth r4, -0x2(r5)
    cmplwi r0, 0xa
    bgt @L_80089E0C
    li r3, -0x1
@L_80089E0C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
