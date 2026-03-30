    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x28(r1)
    stw r31, 0x24(r1)
    stw r30, 0x20(r1)
    stw r29, 0x1c(r1)
    mr r30, r4
    lis r3, 0xcc00
    lwz r31, 0x3000(r3)
    rlwinm r31, r31, 0, 16, 14
    cmplwi r31, 0x0
    beq @L_8009E458
    addi r3, r3, 0x3000
    lwz r0, 0x4(r3)
    and r0, r31, r0
    cmplwi r0, 0x0
    bne @L_8009E460
@L_8009E458:
    mr r3, r30
    bl OSLoadContext
@L_8009E460:
    rlwinm r0, r31, 0, 24, 24
    cmplwi r0, 0x0
    li r0, 0x0
    beq @L_8009E4CC
    lis r3, 0xcc00
    addi r3, r3, 0x4000
    lhz r4, 0x1e(r3)
    rlwinm r3, r4, 0, 31, 31
    cmplwi r3, 0x0
    beq @L_8009E48C
    oris r0, r0, 0x8000
@L_8009E48C:
    rlwinm r3, r4, 0, 30, 30
    cmplwi r3, 0x0
    beq @L_8009E49C
    oris r0, r0, 0x4000
@L_8009E49C:
    rlwinm r3, r4, 0, 29, 29
    cmplwi r3, 0x0
    beq @L_8009E4AC
    oris r0, r0, 0x2000
@L_8009E4AC:
    rlwinm r3, r4, 0, 28, 28
    cmplwi r3, 0x0
    beq @L_8009E4BC
    oris r0, r0, 0x1000
@L_8009E4BC:
    rlwinm r3, r4, 0, 27, 27
    cmplwi r3, 0x0
    beq @L_8009E4CC
    oris r0, r0, 0x800
@L_8009E4CC:
    rlwinm r3, r31, 0, 25, 25
    cmplwi r3, 0x0
    beq @L_8009E514
    lis r3, 0xcc00
    addi r3, r3, 0x5000
    lhz r4, 0xa(r3)
    rlwinm r3, r4, 0, 28, 28
    cmplwi r3, 0x0
    beq @L_8009E4F4
    oris r0, r0, 0x400
@L_8009E4F4:
    rlwinm r3, r4, 0, 26, 26
    cmplwi r3, 0x0
    beq @L_8009E504
    oris r0, r0, 0x200
@L_8009E504:
    rlwinm r3, r4, 0, 24, 24
    cmplwi r3, 0x0
    beq @L_8009E514
    oris r0, r0, 0x100
@L_8009E514:
    rlwinm r3, r31, 0, 26, 26
    cmplwi r3, 0x0
    beq @L_8009E538
    lis r3, 0xcc00
    lwz r3, 0x6c00(r3)
    rlwinm r3, r3, 0, 28, 28
    cmplwi r3, 0x0
    beq @L_8009E538
    oris r0, r0, 0x80
@L_8009E538:
    rlwinm r3, r31, 0, 27, 27
    cmplwi r3, 0x0
    beq @L_8009E5E4
    lis r3, 0xcc00
    lwz r4, 0x6800(r3)
    rlwinm r3, r4, 0, 30, 30
    cmplwi r3, 0x0
    beq @L_8009E55C
    oris r0, r0, 0x40
@L_8009E55C:
    rlwinm r3, r4, 0, 28, 28
    cmplwi r3, 0x0
    beq @L_8009E56C
    oris r0, r0, 0x20
@L_8009E56C:
    rlwinm r3, r4, 0, 20, 20
    cmplwi r3, 0x0
    beq @L_8009E57C
    oris r0, r0, 0x10
@L_8009E57C:
    lis r3, 0xcc00
    addi r3, r3, 0x6800
    lwz r4, 0x14(r3)
    rlwinm r3, r4, 0, 30, 30
    cmplwi r3, 0x0
    beq @L_8009E598
    oris r0, r0, 0x8
@L_8009E598:
    rlwinm r3, r4, 0, 28, 28
    cmplwi r3, 0x0
    beq @L_8009E5A8
    oris r0, r0, 0x4
@L_8009E5A8:
    rlwinm r3, r4, 0, 20, 20
    cmplwi r3, 0x0
    beq @L_8009E5B8
    oris r0, r0, 0x2
@L_8009E5B8:
    lis r3, 0xcc00
    addi r3, r3, 0x6800
    lwz r4, 0x28(r3)
    rlwinm r3, r4, 0, 30, 30
    cmplwi r3, 0x0
    beq @L_8009E5D4
    oris r0, r0, 0x1
@L_8009E5D4:
    rlwinm r3, r4, 0, 28, 28
    cmplwi r3, 0x0
    beq @L_8009E5E4
    ori r0, r0, 0x8000
@L_8009E5E4:
    rlwinm r3, r31, 0, 18, 18
    cmplwi r3, 0x0
    beq @L_8009E5F4
    ori r0, r0, 0x20
@L_8009E5F4:
    rlwinm r3, r31, 0, 19, 19
    cmplwi r3, 0x0
    beq @L_8009E604
    ori r0, r0, 0x40
@L_8009E604:
    rlwinm r3, r31, 0, 21, 21
    cmplwi r3, 0x0
    beq @L_8009E614
    ori r0, r0, 0x1000
@L_8009E614:
    rlwinm r3, r31, 0, 22, 22
    cmplwi r3, 0x0
    beq @L_8009E624
    ori r0, r0, 0x2000
@L_8009E624:
    rlwinm r3, r31, 0, 23, 23
    cmplwi r3, 0x0
    beq @L_8009E634
    ori r0, r0, 0x80
@L_8009E634:
    rlwinm r3, r31, 0, 28, 28
    cmplwi r3, 0x0
    beq @L_8009E644
    ori r0, r0, 0x800
@L_8009E644:
    rlwinm r3, r31, 0, 29, 29
    cmplwi r3, 0x0
    beq @L_8009E654
    ori r0, r0, 0x400
@L_8009E654:
    rlwinm r3, r31, 0, 30, 30
    cmplwi r3, 0x0
    beq @L_8009E664
    ori r0, r0, 0x200
@L_8009E664:
    rlwinm r3, r31, 0, 20, 20
    cmplwi r3, 0x0
    beq @L_8009E674
    ori r0, r0, 0x4000
@L_8009E674:
    rlwinm r3, r31, 0, 31, 31
    cmplwi r3, 0x0
    beq @L_8009E684
    ori r0, r0, 0x100
@L_8009E684:
    lis r3, 0x8000
    lwz r4, 0xc4(r3)
    lwz r3, 0xc8(r3)
    or r3, r4, r3
    andc r4, r0, r3
    cmplwi r4, 0x0
    beq @L_8009E734
    lis r3, 0x8031
    addi r0, r3, 6120
    mr r3, r0
    b @L_8009E6B0
@L_8009E6B0:
    b @L_8009E6B4
@L_8009E6B4:
    lwz r0, 0x0(r3)
    and r0, r4, r0
    cmplwi r0, 0x0
    beq @L_8009E6D0
    cntlzw r0, r0
    extsh r29, r0
    b @L_8009E6D8
@L_8009E6D0:
    addi r3, r3, 0x4
    b @L_8009E6B4
@L_8009E6D8:
    lwz r3, -24848(r13)
    rlwinm r0, r29, 2, 0, 29
    lwzx r31, r3, r0
    cmplwi r31, 0x0
    beq @L_8009E734
    cmpwi r29, 0x4
    ble @L_8009E70C
    sth r29, -24840(r13)
    bl OSGetTime
    stw r4, -24828(r13)
    stw r3, -24832(r13)
    lwz r0, 0x198(r30)
    stw r0, -24844(r13)
@L_8009E70C:
    bl OSDisableScheduler
    mr r3, r29
    mr r4, r30
    mr r12, r31
    mtlr r12
    blrl
    bl OSEnableScheduler
    bl __OSReschedule
    mr r3, r30
    bl OSLoadContext
@L_8009E734:
    mr r3, r30
    bl OSLoadContext
    lwz r0, 0x2c(r1)
    lwz r31, 0x24(r1)
    lwz r30, 0x20(r1)
    lwz r29, 0x1c(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr