    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    lis r3, -32698
    li r5, 0x0
    addi r31, r3, 29584
    lwz r0, 0x0(r31)
    cmpwi r0, 0x0
    beq @L_801D266C
    lwz r4, 0x4(r31)
    cmpwi r4, 0x258
    bge @L_801D2474
    lis r3, 0x8889
    subi r0, r3, 0x7777
    mulhw r0, r0, r4
    add r0, r0, r4
    srawi r0, r0, 5
    rlwinm r3, r0, 1, 31, 31
    add r0, r0, r3
    mulli r0, r0, 0x3c
    subf r0, r0, r4
    cmpwi r0, 0x1e
    bge @L_801D2474
    li r5, 0x1
@L_801D2474:
    cmpwi r5, 0x0
    beq @L_801D25C8
    lis r3, 0xc88
    addi r3, r3, 0x1200
    bl fn_800F92D4
    mr r28, r3
    bl fn_800EF4FC
    mr r29, r3
    mr r3, r28
    bl fn_800EF4F4
    mr r30, r3
    li r3, 0x0
    li r4, 0x0
    bl fn_800FE6D0
    bl fn_800FE35C
    bl fn_800FE4D4
    li r3, 0x1
    bl fn_800D9ED8
    lis r3, -32719
    addi r3, r3, 20376
    bl fn_800D7820
    li r3, 0x3
    bl fn_800D88DC
    li r3, 0x4
    bl fn_800D888C
    mr r4, r28
    li r3, 0x0
    bl fn_800D85D4
    li r3, 0x7
    bl fn_800D6A00
    li r3, 0x2
    bl fn_800D67BC
    li r3, 0x15
    li r4, 0x25
    bl fn_800D61E4
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    li r7, 0xff
    bl fn_800D5CB8
    lfs f1, -21740(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    addi r3, r29, 0x15
    addi r0, r30, 0x25
    extsh r3, r3
    extsh r4, r0
    bl fn_800D61E4
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    li r7, 0xff
    bl fn_800D5CB8
    lfs f1, -21736(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    bl fn_800D6728
    li r3, 0x0
    bl fn_800D9ED8
    li r3, 0x0
    li r4, 0x9
    bl fn_80135168
    cmpwi r3, 0x0
    bne @L_801D25C8
    lwz r3, 0x4(r31)
    lis r0, 0x4330
    stw r0, 0x8(r1)
    xoris r0, r3, 0x8000
    lfd f2, -21728(r2)
    stw r0, 0xc(r1)
    lfs f0, -21732(r2)
    lfd f1, 0x8(r1)
    fsubs f1, f1, f2
    fcmpo cr0, f1, f0
    bge @L_801D25C8
    li r3, 0x1
    li r4, 0x0
    li r5, 0xff
    li r6, 0x1e
    li r7, 0x0
    bl fn_800F78A4
@L_801D25C8:
    bl fn_800D3088
    lis r4, -32698
    lwz r0, 0x4(r31)
    addi r30, r4, 29584
    lwz r28, 0x8(r30)
    add r0, r0, r3
    stw r0, 0x4(r31)
    cmplwi r28, 0x0
    beq @L_801D2644
    mr r3, r28
    bl fn_801666BC
    cmpwi r3, 0x2
    beq @L_801D2644
    li r3, 0x0
    li r4, 0x408
    bl fn_800F9210
    mr r3, r28
    bl fn_80166B18
    lis r3, -32698
    addi r3, r3, 29584
    lwz r3, 0xc(r3)
    cmplwi r3, 0x0
    beq @L_801D263C
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    beq @L_801D263C
    li r4, 0x0
    li r5, 0x0
    bl fn_801667D8
@L_801D263C:
    li r0, 0x0
    stw r0, 0x8(r30)
@L_801D2644:
    lwz r0, 0x4(r31)
    cmpwi r0, 0x258
    blt @L_801D266C
    lwz r0, 0x8(r30)
    cmplwi r0, 0x0
    bne @L_801D266C
    lis r3, -32698
    li r0, 0x0
    addi r3, r3, 29584
    stw r0, 0x0(r3)
@L_801D266C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
