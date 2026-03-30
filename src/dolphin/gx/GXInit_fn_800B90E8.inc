    nofralloc
    mflr r0
    li r3, 0x1
    stw r0, 0x4(r1)
    stwu r1, -0x2e0(r1)
    stw r31, 0x2dc(r1)
    addi r31, r4, 0x0
    lwz r5, -24224(r13)
    lhz r0, 0xa(r5)
    rlwinm r0, r0, 0, 29, 27
    ori r0, r0, 0x8
    sth r0, 0xa(r5)
    lwz r0, -24156(r13)
    stb r3, -24152(r13)
    cmplwi r0, 0x0
    beq @L_800B9150
    addi r3, r1, 0x10
    bl OSClearContext
    addi r3, r1, 0x10
    bl OSSetCurrentContext
    lwz r12, -24156(r13)
    mtlr r12
    blrl
    addi r3, r1, 0x10
    bl OSClearContext
    mr r3, r31
    bl OSSetCurrentContext
@L_800B9150:
    addi r3, r13, -24148
    bl fn_800A2478
    lwz r0, 0x2e4(r1)
    lwz r31, 0x2dc(r1)
    addi r1, r1, 0x2e0
    mtlr r0
    blr
