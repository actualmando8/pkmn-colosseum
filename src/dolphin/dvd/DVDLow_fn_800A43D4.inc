    nofralloc
    mflr r0
    lis r7, CommandList_803FC290@ha
    stw r0, 0x4(r1)
    clrrwi. r8, r5, 15
    addi r9, r7, CommandList_803FC290@l
    stwu r1, -0x8(r1)
    bne @L_800A43F8
    li r10, 0x0
    b @L_800A4400
    @L_800A43F8:
    lwz r0, WorkAroundSeekLocation_8047A7A8(r13)
    add r10, r8, r0
    @L_800A4400:
    li r0, 0x2
    stw r0, 0x0(r9)
    li r8, 0x1
    li r7, -0x1
    stw r10, 0xc(r9)
    li r0, 0x0
    stw r6, 0x10(r9)
    stw r8, 0x14(r9)
    stw r3, 0x18(r9)
    mr r3, r10
    stw r4, 0x1c(r9)
    mr r4, r6
    stw r5, 0x20(r9)
    stw r6, 0x24(r9)
    stw r7, 0x28(r9)
    stw r0, NextCommandNumber_8047A7C4(r13)
    bl fn_800A46EC
    lwz r0, 0xc(r1)
    addi r1, r1, 0x8
    mtlr r0
    blr
