    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    b @L_801BDA3C
    @L_801BD8E8:
    lwz r0, 0xc(r31)
    cmpwi r0, 0xff
    beq @L_801BDA38
    lwz r0, 0x4c(r31)
    rlwinm. r0, r0, 0, 7, 7
    beq @L_801BD9CC
    bl fn_801B37A0
    lis r4, lbl_8036D510@ha
    lis r8, lbl_8036D510@ha
    addi r4, r4, lbl_8036D510@l
    lis r7, lbl_8036D510@ha
    stw r3, 0x8(r4)
    lis r5, lbl_8036D510@ha
    lis r4, lbl_8036D510@ha
    lis r3, lbl_8036D510@ha
    lwz r0, 0xa4(r31)
    addi r8, r8, lbl_8036D510@l
    lis r6, lbl_8036D510@ha
    addi r9, r7, lbl_8036D510@l
    stw r0, 0xc(r8)
    addi r7, r6, lbl_8036D510@l
    addi r5, r5, lbl_8036D510@l
    addi r4, r4, lbl_8036D510@l
    lwz r10, 0xc(r31)
    li r8, 0x0
    li r6, 0x0
    li r0, 0x0
    stw r10, 0x10(r9)
    addi r3, r3, lbl_8036D510@l
    stw r8, 0x18(r7)
    stw r6, 0x3c(r5)
    stb r0, 0x34(r4)
    bl fn_801B3638
    bl fn_801B37A0
    lis r4, lbl_8036D510@ha
    lis r7, lbl_8036D510@ha
    addi r4, r4, lbl_8036D510@l
    lis r5, lbl_8036D510@ha
    stw r3, 0x8(r4)
    lis r4, lbl_8036D510@ha
    lis r3, lbl_8036D510@ha
    lis r6, lbl_8036D510@ha
    lwz r10, 0xa4(r31)
    addi r9, r7, lbl_8036D510@l
    addi r7, r6, lbl_8036D510@l
    li r8, 0x1
    addi r10, r10, 0x1
    addi r5, r5, lbl_8036D510@l
    li r6, 0x1
    addi r4, r4, lbl_8036D510@l
    li r0, 0x1
    stw r10, 0xc(r9)
    addi r3, r3, lbl_8036D510@l
    stw r8, 0x18(r7)
    stw r6, 0x3c(r5)
    stb r0, 0x34(r4)
    bl fn_801B3638
    @L_801BD9CC:
    lwz r0, 0x4c(r31)
    rlwinm. r0, r0, 0, 23, 23
    beq @L_801BDA38
    b @L_801BDA1C
    @L_801BD9DC:
    bl fn_801B37A0
    lis r4, lbl_8036D48C@ha
    lis r6, lbl_8036D48C@ha
    addi r7, r4, lbl_8036D48C@l
    lis r5, lbl_8036D48C@ha
    stw r3, 0x8(r7)
    lis r4, lbl_8036D48C@ha
    addi r3, r4, lbl_8036D48C@l
    addi r6, r6, lbl_8036D48C@l
    lwz r0, 0xa4(r31)
    addi r4, r5, lbl_8036D48C@l
    stw r0, 0xc(r6)
    lwz r0, 0xc(r31)
    stw r0, 0x10(r4)
    bl fn_801B3638
    lwz r31, 0x8(r31)
    @L_801BDA1C:
    cmplwi r31, 0x0
    beq @L_801BDA44
    lwz r0, 0x4c(r31)
    clrlwi r0, r0, 28
    cmplwi r0, 0x3
    beq @L_801BD9DC
    b @L_801BDA44
    @L_801BDA38:
    lwz r31, 0x8(r31)
    @L_801BDA3C:
    cmplwi r31, 0x0
    bne @L_801BD8E8
    @L_801BDA44:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
